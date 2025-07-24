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

#include <utility>
#include <vector>
#include <string>
#include <cstring>
#include <Functions/Meos/TemporalIntersectsGeometryPhysicalFunction.hpp>
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

VarVal TemporalIntersectsGeometryPhysicalFunction::execute(const Record& record, ArenaRef& arena) const
{
    const auto geometryStringValue = geometryStringFunction.execute(record, arena);
    
    std::cout << "TemporalIntersectsGeometryPhysicalFunction::execute called" << std::endl;
    
    // Extract VariableSizedData value from VarVal 
    auto geometry_varsized_val = geometryStringValue.cast<NES::Nautilus::VariableSizedData>();
    
    std::cout << "TemporalIntersectsGeometryPhysicalFunction processing variable-sized string value" << std::endl;
    
    // Use nautilus::invoke to call external MEOS function with variable-sized data
    const auto result = nautilus::invoke(
        +[](int8_t* data_ptr, uint32_t data_size) -> bool {
            try {
                // Use the existing global MEOS initialization mechanism
                MEOS::Meos::ensureMeosInitialized();
                
                // Convert raw data to C++ string
                std::string geometry_wkt(reinterpret_cast<const char*>(data_ptr), data_size);
                std::cout << "Processing temporal geometry WKT: " << geometry_wkt << std::endl;
                
                // Strip quotes if present (CSV parsing includes quotes in the string value)
                if (geometry_wkt.size() >= 2 && geometry_wkt.front() == '\'' && geometry_wkt.back() == '\'') {
                    geometry_wkt = geometry_wkt.substr(1, geometry_wkt.size() - 2);
                    std::cout << "Stripped quotes, processed WKT: " << geometry_wkt << std::endl;
                }
                
                // Validate input string is not empty
                if (geometry_wkt.empty()) {
                    std::cout << "Empty geometry WKT string" << std::endl;
                    return false;
                }
                
                // Create temporal geometry from input WKT string
                MEOS::Meos::TemporalGeometry input_geometry(geometry_wkt);
                
                // Create hardcoded reference temporal geometry for intersection testing
                // Example: temporal polygon that changes over time in Manhattan area
                std::string reference_temporal_geom = 
                    "SRID=4326;{POLYGON((-73.9900 40.7450, -73.9800 40.7450, -73.9800 40.7500, -73.9900 40.7500, -73.9900 40.7450))@2021-01-01 00:00:00, "
                    "POLYGON((-73.9850 40.7475, -73.9750 40.7475, -73.9750 40.7525, -73.9850 40.7525, -73.9850 40.7475))@2021-01-01 12:00:00}";
                
                std::cout << "Reference temporal geometry for comparison: " << reference_temporal_geom << std::endl;
                MEOS::Meos::TemporalGeometry reference_geometry(reference_temporal_geom);
                
                std::cout << "About to compare:" << std::endl;
                std::cout << "  Input: " << geometry_wkt << std::endl;
                std::cout << "  Reference: " << reference_temporal_geom << std::endl;
                
                // Perform temporal geometry vs temporal geometry intersection using eintersects_tgeo_tgeo
                bool intersection_result = input_geometry.intersects(reference_geometry);
                std::cout << "MEOS temporal geometry intersection result: " << intersection_result << std::endl;

                return intersection_result;
            } catch (const std::exception& e) {
                std::cout << "MEOS exception in temporal geometry intersection: " << e.what() << std::endl;
                return false;
            } catch (...) {
                std::cout << "Unknown error in temporal geometry intersection" << std::endl;
                return false;
            }
        },
        geometry_varsized_val.getContent(),
        geometry_varsized_val.getContentSize()
    );
    
    return VarVal(result);
}

TemporalIntersectsGeometryPhysicalFunction::TemporalIntersectsGeometryPhysicalFunction(PhysicalFunction geometryStringFunction)
    : geometryStringFunction(std::move(geometryStringFunction))
{
}

PhysicalFunctionRegistryReturnType
PhysicalFunctionGeneratedRegistrar::RegisterTemporalIntersectsGeometryPhysicalFunction(PhysicalFunctionRegistryArguments physicalFunctionRegistryArguments)
{
    PRECONDITION(physicalFunctionRegistryArguments.childFunctions.size() == 1, "TemporalIntersectsGeometry function must have exactly one sub-function");
    return TemporalIntersectsGeometryPhysicalFunction(physicalFunctionRegistryArguments.childFunctions[0]);
}

}