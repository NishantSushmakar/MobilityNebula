/*
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        https://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <Aggregation/Function/Meos/TemporalSequenceAggregationPhysicalFunction.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <string_view>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <cstring>
#include <cstdio>

#include <MemoryLayout/ColumnLayout.hpp>
#include <Nautilus/Interface/MemoryProvider/ColumnTupleBufferMemoryProvider.hpp>
#include <Nautilus/Interface/MemoryProvider/TupleBufferMemoryProvider.hpp>
#include <Nautilus/Interface/PagedVector/PagedVector.hpp>
#include <Nautilus/Interface/PagedVector/PagedVectorRef.hpp>
#include <Nautilus/Interface/Record.hpp>
#include <nautilus/function.hpp>

#include <AggregationPhysicalFunctionRegistry.hpp>
#include <ErrorHandling.hpp>
#include <val.hpp>
#include <val_concepts.hpp>
#include <val_ptr.hpp>

// MEOS C API headers
extern "C" {
#include <meos.h>
#include <meos_geo.h>
}

namespace NES
{

constexpr static std::string_view LonFieldName = "lon";
constexpr static std::string_view LatFieldName = "lat";
constexpr static std::string_view TimestampFieldName = "timestamp";

// Global MEOS initialization
static bool meos_initialized = false;
static std::mutex meos_mutex;

static void ensureMeosInitialized() {
    std::lock_guard<std::mutex> lock(meos_mutex);
    if (!meos_initialized) {
        // Set timezone to UTC if not set
        if (!std::getenv("TZ")) {
            setenv("TZ", "UTC", 1);
            tzset();
        }
        meos_initialize();
        meos_initialized = true;
    }
}

// Single-field constructor removed - TEMPORAL_SEQUENCE requires three separate field functions

TemporalSequenceAggregationPhysicalFunction::TemporalSequenceAggregationPhysicalFunction(
    DataType inputType,
    DataType resultType,
    PhysicalFunction lonFunctionParam,
    PhysicalFunction latFunctionParam,
    PhysicalFunction timestampFunctionParam,
    Nautilus::Record::RecordFieldIdentifier resultFieldIdentifier,
    std::shared_ptr<Nautilus::Interface::MemoryProvider::TupleBufferMemoryProvider> memProviderPagedVector)
    : AggregationPhysicalFunction(std::move(inputType), std::move(resultType), lonFunctionParam, std::move(resultFieldIdentifier))
    , memProviderPagedVector(std::move(memProviderPagedVector))
    , lonFunction(std::move(lonFunctionParam))
    , latFunction(std::move(latFunctionParam))
    , timestampFunction(std::move(timestampFunctionParam))
{
}

void TemporalSequenceAggregationPhysicalFunction::lift(
    const nautilus::val<AggregationState*>& aggregationState, ExecutionContext& executionContext, const Nautilus::Record& record)
{
    // Debug: Print when lift is called
    printf("DEBUG: TEMPORAL_SEQUENCE lift() called - adding point to aggregation\n");
    
    // Cast to PagedVector pointer consistently with combine() and lower()
    const auto pagedVectorPtr = static_cast<nautilus::val<Nautilus::Interface::PagedVector*>>(aggregationState);
    
    // For TEMPORAL_SEQUENCE, we need to store lon, lat, and timestamp values
    auto lonValue = lonFunction.execute(record, executionContext.pipelineMemoryProvider.arena);
    auto latValue = latFunction.execute(record, executionContext.pipelineMemoryProvider.arena);
    auto timestampValue = timestampFunction.execute(record, executionContext.pipelineMemoryProvider.arena);
    
    // Create a record with all three fields for temporal sequence
    Record aggregateStateRecord({
        {std::string(LonFieldName), lonValue},
        {std::string(LatFieldName), latValue},
        {std::string(TimestampFieldName), timestampValue}
    });
    
    const Nautilus::Interface::PagedVectorRef pagedVectorRef(pagedVectorPtr, memProviderPagedVector);
    pagedVectorRef.writeRecord(aggregateStateRecord, executionContext.pipelineMemoryProvider.bufferProvider);
}

void TemporalSequenceAggregationPhysicalFunction::combine(
    const nautilus::val<AggregationState*> aggregationState1,
    const nautilus::val<AggregationState*> aggregationState2,
    PipelineMemoryProvider&)
{
    /// Getting the paged vectors from the aggregation states
    const auto memArea1 = static_cast<nautilus::val<Nautilus::Interface::PagedVector*>>(aggregationState1);
    const auto memArea2 = static_cast<nautilus::val<Nautilus::Interface::PagedVector*>>(aggregationState2);

    /// Calling the copyFrom function of the paged vector to combine the two paged vectors by copying the content of the second paged vector to the first paged vector
    nautilus::invoke(
        +[](Nautilus::Interface::PagedVector* vector1, const Nautilus::Interface::PagedVector* vector2) -> void
        { vector1->copyFrom(*vector2); },
        memArea1,
        memArea2);
}

Nautilus::Record TemporalSequenceAggregationPhysicalFunction::lower(
    const nautilus::val<AggregationState*> aggregationState, PipelineMemoryProvider& pipelineMemoryProvider)
{
    // Ensure MEOS is initialized
    ensureMeosInitialized();
    
    /// Getting the paged vector from the aggregation state
    const auto pagedVectorPtr = static_cast<nautilus::val<Nautilus::Interface::PagedVector*>>(aggregationState);
    const Nautilus::Interface::PagedVectorRef pagedVectorRef(pagedVectorPtr, memProviderPagedVector);
    const auto allFieldNames = memProviderPagedVector->getMemoryLayout()->getSchema().getFieldNames();
    const auto numberOfEntries = invoke(
        +[](const Nautilus::Interface::PagedVector* pagedVector)
        {
            auto count = pagedVector->getTotalNumberOfEntries();
            printf("DEBUG: PagedVector contains %lu entries\n", (unsigned long)count);
            return count;
        },
        pagedVectorPtr);

    // Handle empty PagedVector case
    // For now, we'll handle empty case by creating an empty trajectory string
    // TODO: Consider proper handling of empty trajectories in MEOS

    // Build the trajectory string in MEOS format for temporal instant set
    // For single point: POINT(-73.9857 40.7484)@2000-01-01 08:00:00
    // For multiple points: {POINT(-73.9857 40.7484)@2000-01-01 08:00:00, POINT(-73.9787 40.7505)@2000-01-01 08:05:00}
    auto trajectoryStr = nautilus::invoke(
        +[](const Nautilus::Interface::PagedVector* pagedVector) -> char*
        {
            // Allocate a buffer for the trajectory string
            // Each point is approximately 100 chars: POINT(-123.456789 12.345678)@2000-01-01 08:00:00
            size_t bufferSize = pagedVector->getTotalNumberOfEntries() * 100 + 20;
            char* buffer = (char*)malloc(bufferSize);
            
            // Start with opening brace for multiple points (will handle single point case later)
            strcpy(buffer, "{");
            return buffer;
        },
        pagedVectorPtr);

    // Track if this is the first point using a counter
    auto pointCounter = nautilus::val<int64_t>(0);
    
    /// Read from paged vector
    const auto endIt = pagedVectorRef.end(allFieldNames);
    for (auto candidateIt = pagedVectorRef.begin(allFieldNames); candidateIt != endIt; ++candidateIt)
    {
        const auto itemRecord = *candidateIt;
        
        // Read all three fields for temporal sequence
        const auto lonValue = itemRecord.read(std::string(LonFieldName));
        const auto latValue = itemRecord.read(std::string(LatFieldName));
        const auto timestampValue = itemRecord.read(std::string(TimestampFieldName));
        
        // Use cast to extract values from VarVal
        auto lon = lonValue.cast<nautilus::val<double>>();
        auto lat = latValue.cast<nautilus::val<double>>();
        auto timestamp = timestampValue.cast<nautilus::val<int64_t>>();
        
        // Append point to trajectory string in MEOS format
        trajectoryStr = nautilus::invoke(
            +[](char* buffer, double lonVal, double latVal, int64_t tsVal, int64_t counter) -> char*
            {
                printf("DEBUG: Processing point %ld: POINT(%.6f %.6f)@adjusted_timestamp_%ld\n", counter, lonVal, latVal, tsVal);
                
                if (counter > 0) {
                    strcat(buffer, ", ");
                }
                
                // Convert timestamp to proper MEOS format
                // Assuming tsVal is in milliseconds since epoch, convert to proper datetime
                // For MEOS, we need a more realistic timestamp, not 1970-01-01
                // Let's use 2000-01-01 as base and add the timestamp as seconds
                time_t baseTime = 946684800; // 2000-01-01 00:00:00 UTC
                time_t adjustedTime = baseTime + (tsVal / 1000);  // Add seconds
                struct tm* timeinfo = gmtime(&adjustedTime);
                
                char timestampStr[32];
                strftime(timestampStr, sizeof(timestampStr), "%Y-%m-%d %H:%M:%S", timeinfo);
                
                char pointStr[120];
                // Use POINT format that MEOS expects: POINT(lon lat) instead of Point(lon lat)
                sprintf(pointStr, "POINT(%.6f %.6f)@%s", lonVal, latVal, timestampStr);
                strcat(buffer, pointStr);
                
                printf("DEBUG: Added point: %s\n", pointStr);
                return buffer;
            },
            trajectoryStr,
            lon,
            lat,
            timestamp,
            pointCounter);
            
        pointCounter = pointCounter + nautilus::val<int64_t>(1);
    }
    
    // Close the trajectory string and handle single vs multiple points
    trajectoryStr = nautilus::invoke(
        +[](char* buffer, int64_t totalPoints) -> char*
        {
            if (totalPoints == 1) {
                // For single point, remove the opening brace
                // Move content left by 1 character to remove the '{'
                memmove(buffer, buffer + 1, strlen(buffer));
                printf("DEBUG: Single point trajectory string: %s\n", buffer);
            } else {
                // For multiple points, close with brace
                strcat(buffer, "}");
                printf("DEBUG: Multiple points trajectory string: %s\n", buffer);
            }
            return buffer;
        },
        trajectoryStr,
        pointCounter);
    
    // Convert string to MEOS binary format
    // For temporal instant sets, we use tgeompoint_in() which can handle both single instants and instant sets
    auto binarySize = nautilus::invoke(
        +[](const char* trajStr) -> size_t
        {
            printf("DEBUG: Converting temporal instant string to MEOS binary format\n");
            printf("DEBUG: Input temporal instant string: %s\n", trajStr);
            
            // Parse the temporal instant string into a MEOS temporal object
            // tgeompoint_in() can handle both single instants and instant sets
            printf("DEBUG: Calling tgeompoint_in()...\n");
            Temporal* temp = tgeompoint_in(trajStr);
            if (!temp) {
                printf("ERROR: Failed to parse temporal instant string: %s\n", trajStr);
                printf("ERROR: tgeompoint_in() returned NULL\n");
                printf("ERROR: Check if the format matches MEOS expectations for temporal instants\n");
                return 0;
            }
            printf("DEBUG: Successfully parsed temporal instant string into MEOS Temporal object\n");
            
            // Get the size needed for binary WKB format
            // WKB_EXTENDED = 0x08 for extended WKB format
            printf("DEBUG: Calling temporal_as_wkb() to get binary size...\n");
            size_t size = 0;
            uint8_t* data = temporal_as_wkb(temp, 0x08, &size);
            
            if (!data) {
                printf("ERROR: temporal_as_wkb() returned NULL\n");
                free(temp);
                return 0;
            }
            
            printf("DEBUG: temporal_as_wkb() successful, binary size: %zu bytes\n", size);
            free(data);
            free(temp);
            
            return size;
        },
        trajectoryStr);
    
    // Allocate variable sized data for the binary result
    auto variableSized = pipelineMemoryProvider.arena.allocateVariableSizedData(binarySize);
    
    // Now convert and copy the binary data directly to the allocated memory
    nautilus::invoke(
        +[](int8_t* dest, const char* trajStr, size_t expectedSize) -> void
        {
            printf("DEBUG: Second MEOS conversion - converting temporal instant and copying binary data\n");
            printf("DEBUG: Expected size: %zu bytes\n", expectedSize);
            
            // Parse the temporal instant string into a MEOS temporal object
            printf("DEBUG: Calling tgeompoint_in() (second time)...\n");
            Temporal* temp = tgeompoint_in(trajStr);
            if (!temp) {
                printf("ERROR: Failed to parse temporal instant string on second attempt\n");
                printf("ERROR: Input was: %s\n", trajStr);
                free((void*)trajStr);
                return;
            }
            printf("DEBUG: Successfully parsed temporal instant string (second time)\n");
            
            // Convert to binary WKB format
            printf("DEBUG: Calling temporal_as_wkb() (second time)...\n");
            size_t actualSize = 0;
            uint8_t* binaryData = temporal_as_wkb(temp, 0x08, &actualSize);
            
            if (!binaryData) {
                printf("ERROR: temporal_as_wkb() returned NULL on second attempt\n");
                free(temp);
                free((void*)trajStr);
                return;
            }
            
            if (actualSize != expectedSize) {
                printf("ERROR: Size mismatch - expected %zu, got %zu\n", expectedSize, actualSize);
                free(binaryData);
                free(temp);
                free((void*)trajStr);
                return;
            }
            
            printf("DEBUG: temporal_as_wkb() successful (second time), actual size: %zu bytes\n", actualSize);
            
            // Copy the binary data
            memcpy(dest, binaryData, actualSize);
            printf("DEBUG: Successfully copied %zu bytes of binary temporal instant data\n", actualSize);
            
            // Print first few bytes of binary data for verification
            printf("DEBUG: First 16 bytes of binary data: ");
            for (size_t i = 0; i < 16 && i < actualSize; i++) {
                printf("%02X ", (unsigned char)binaryData[i]);
            }
            printf("\n");
            
            // Clean up
            free(binaryData);
            free(temp);
            free((void*)trajStr);
            printf("DEBUG: MEOS conversion and cleanup complete\n");
        },
        variableSized.getContent(),
        trajectoryStr,
        binarySize);

    Nautilus::Record resultRecord;
    resultRecord.write(resultFieldIdentifier, variableSized);

    return resultRecord;
}

void TemporalSequenceAggregationPhysicalFunction::reset(const nautilus::val<AggregationState*> aggregationState, PipelineMemoryProvider&)
{
    nautilus::invoke(
        +[](AggregationState* pagedVectorMemArea) -> void
        {
            /// Allocates a new PagedVector in the memory area provided by the pointer to the pagedvector
            auto* pagedVector = reinterpret_cast<Nautilus::Interface::PagedVector*>(pagedVectorMemArea);
            new (pagedVector) Nautilus::Interface::PagedVector();
        },
        aggregationState);
}

size_t TemporalSequenceAggregationPhysicalFunction::getSizeOfStateInBytes() const
{
    return sizeof(Nautilus::Interface::PagedVector);
}
void TemporalSequenceAggregationPhysicalFunction::cleanup(nautilus::val<AggregationState*> aggregationState)
{
    nautilus::invoke(
        +[](AggregationState* pagedVectorMemArea) -> void
        {
            /// Calls the destructor of the PagedVector
            auto* pagedVector = reinterpret_cast<Nautilus::Interface::PagedVector*>(
                pagedVectorMemArea); /// NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
            pagedVector->~PagedVector();
        },
        aggregationState);
}


AggregationPhysicalFunctionRegistryReturnType AggregationPhysicalFunctionGeneratedRegistrar::RegisterTemporalSequenceAggregationPhysicalFunction(
    AggregationPhysicalFunctionRegistryArguments)
{
    throw std::runtime_error("TEMPORAL_SEQUENCE aggregation cannot be created through the registry. "
                           "It requires three field functions (longitude, latitude, timestamp) ");
}

}
