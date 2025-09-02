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

#include <Functions/Meos/TcontainsGeoTgeoPhysicalFunction.hpp>
#include <Functions/PhysicalFunction.hpp>
#include <Nautilus/DataTypes/VarVal.hpp>
#include <Nautilus/DataTypes/VariableSizedData.hpp>
#include <Nautilus/Interface/Record.hpp>
#include <ErrorHandling.hpp>
#include <ExecutionContext.hpp>
#include <PhysicalFunctionRegistry.hpp>
#include <MEOSWrapper.hpp>
#include <fmt/format.h>
#include <iostream>
#include <val.hpp>
#include <function.hpp>

namespace NES {

TcontainsGeoTgeoPhysicalFunction::TcontainsGeoTgeoPhysicalFunction(        PhysicalFunction param0Function,
        PhysicalFunction param1Function,
        PhysicalFunction param2Function,
        PhysicalFunction param3Function)
{
    parameterFunctions.reserve(4);
    parameterFunctions.push_back(std::move(param0Function));
    parameterFunctions.push_back(std::move(param1Function));
    parameterFunctions.push_back(std::move(param2Function));
    parameterFunctions.push_back(std::move(param3Function));

}

VarVal TcontainsGeoTgeoPhysicalFunction::execute(const Record& record, ArenaRef& arena) const
{
    
    // Execute all parameter functions to get their values
    std::vector<VarVal> parameterValues;
    parameterValues.reserve(4);
    for (const auto& paramFunc : parameterFunctions) {
        parameterValues.push_back(paramFunc.execute(record, arena));
    }
    
    
    auto param0_varsized = parameterValues[0].cast<VariableSizedData>();
    auto param1_varsized = parameterValues[1].cast<VariableSizedData>();
    auto param2 = parameterValues[2].cast<nautilus::val<int32_t>>();
    auto param3 = parameterValues[3].cast<nautilus::val<int32_t>>();

    
    
    // Pre-allocate result buffer
    auto resultData = arena.allocateVariableSizedData(4096); // Max buffer size
    
    // Use nautilus::invoke to call direct MEOS function and write result directly
    const auto result_len = nautilus::invoke(
        +[](const char* param0_ptr, uint32_t param0_size, const char* param1_ptr, uint32_t param1_size, int32_t param2, int32_t param3, int8_t* result_buffer, uint32_t buffer_size) -> int32_t {
            try {
                // Use the existing global MEOS initialization mechanism
                MEOS::Meos::ensureMeosInitialized();
                
                // MEOS Type Conversions
                std::string param0_str(param0_ptr, param0_size);
                printf("DEBUG: param0_str (raw): '%.*s' (size: %u)\n", param0_size, param0_ptr, param0_size);
                
                // Strip outer quotes if present
                if (param0_str.length() >= 2 && param0_str.front() == '\'' && param0_str.back() == '\'') {
                    param0_str = param0_str.substr(1, param0_str.length() - 2);
                    printf("DEBUG: param0_str (unquoted): '%s'\n", param0_str.c_str());
                } else {
                    printf("DEBUG: param0_str (no quotes): '%s'\n", param0_str.c_str());
                }
                
                printf("DEBUG: About to call geom_in with: '%s'\n", param0_str.c_str());
                MEOS::GSERIALIZED* param0_obj = MEOS::geom_in(param0_str.c_str(), -1);
                if (!param0_obj) {
                    printf("DEBUG: geom_in failed for param0\n");
                    return -1; // Error
                }
                printf("DEBUG: geom_in succeeded for param0\n");

                std::string param1_str(param1_ptr, param1_size);
                printf("DEBUG: param1_str (raw): '%.*s' (size: %u)\n", param1_size, param1_ptr, param1_size);
                
                // Strip outer quotes if present
                if (param1_str.length() >= 2 && param1_str.front() == '\'' && param1_str.back() == '\'') {
                    param1_str = param1_str.substr(1, param1_str.length() - 2);
                    printf("DEBUG: param1_str (unquoted): '%s'\n", param1_str.c_str());
                } else {
                    printf("DEBUG: param1_str (no quotes): '%s'\n", param1_str.c_str());
                }
                
                printf("DEBUG: About to call tgeometry_in with: '%s'\n", param1_str.c_str());
                MEOS::Temporal* param1_obj = MEOS::tgeometry_in(param1_str.c_str());
                if (!param1_obj) {
                    printf("DEBUG: tgeometry_in failed for param1\n");
                    // Cleanup param0_obj before returning
                    if (param0_obj) free(param0_obj);
                    return -1; // Error
                }
                printf("DEBUG: tgeometry_in succeeded for param1\n");

                // Convert int32_t parameters to bool for MEOS function
                bool restr = (param2 != 0);
                bool atvalue = (param3 != 0);
                printf("DEBUG: Parameters: restr=%s, atvalue=%s\n", restr ? "true" : "false", atvalue ? "true" : "false");
                
                // Call the direct MEOS function
                printf("DEBUG: About to call tcontains_geo_tgeo\n");
                auto meos_result = MEOS::tcontains_geo_tgeo(param0_obj, param1_obj, restr, atvalue);
                printf("DEBUG: tcontains_geo_tgeo completed, result ptr: %p\n", (void*)meos_result);
                
                // Cleanup input objects
                if (param0_obj) free(param0_obj);
                if (param1_obj) free(param1_obj);
                
                if (!meos_result) {
                    printf("DEBUG: tcontains_geo_tgeo returned null result (no temporal containment found)\n");
                    // Return empty result indicating no containment periods
                    return 0; // Return 0 length string
                }
                
                // Serialize temporal result to string (tcontains returns temporal boolean)
                printf("DEBUG: About to call tbool_out\n");
                char* temporal_str = MEOS::tbool_out(meos_result);
                if (!temporal_str) {
                    printf("DEBUG: tbool_out failed\n");
                    free(meos_result);
                    return -1; // Error
                }
                printf("DEBUG: tbool_out succeeded: '%s'\n", temporal_str);
                
                // Copy to result buffer
                uint32_t str_len = strlen(temporal_str);
                if (str_len > buffer_size) {
                    free(temporal_str);
                    free(meos_result);
                    return -1; // Buffer too small
                }
                
                std::memcpy(result_buffer, temporal_str, str_len);
                
                // Cleanup
                free(temporal_str);
                free(meos_result);
                
                return static_cast<int32_t>(str_len);
                
            } catch (const std::exception& e) {
                // MEOS exception occurred
                return -1; // Error
            } catch (...) {
                // Unknown error occurred
                return -1; // Error
            }
        },
        param0_varsized.getContent(), param0_varsized.getContentSize(),
        param1_varsized.getContent(), param1_varsized.getContentSize(),
        param2,
        param3,
        resultData.getContent(), resultData.getContentSize()
    );
    
    // Check for error
    if (result_len < 0) {
        // Return error - use empty string
        auto errorData = arena.allocateVariableSizedData(0);
        return VarVal(errorData);
    }
    
    // Shrink the buffer to actual size
    resultData.shrink(4096 - result_len);
    
    return VarVal(resultData);
}

// Conversion method implementations will be added based on parameter types

PhysicalFunctionRegistryReturnType
PhysicalFunctionGeneratedRegistrar::RegisterTcontainsGeoTgeoPhysicalFunction(PhysicalFunctionRegistryArguments physicalFunctionRegistryArguments)
{
    PRECONDITION(physicalFunctionRegistryArguments.childFunctions.size() == 4, "TcontainsGeoTgeoPhysicalFunction requires exactly 4 child functions, but got {}", physicalFunctionRegistryArguments.childFunctions.size());
    return TcontainsGeoTgeoPhysicalFunction(physicalFunctionRegistryArguments.childFunctions[0], physicalFunctionRegistryArguments.childFunctions[1], physicalFunctionRegistryArguments.childFunctions[2], physicalFunctionRegistryArguments.childFunctions[3]);
}

} // namespace NES