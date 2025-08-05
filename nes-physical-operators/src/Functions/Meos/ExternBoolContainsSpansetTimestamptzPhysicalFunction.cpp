/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <utility>
#include <vector>
#include <string>
#include <cstring>
#include <Functions/Meos/ExternBoolContainsSpansetTimestamptzPhysicalFunction.hpp>
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

// Constructor with 4 parameters for temporal-static intersection
ExternBoolContainsSpansetTimestamptzPhysicalFunction::ExternBoolContainsSpansetTimestamptzPhysicalFunction(PhysicalFunction lon1Function, PhysicalFunction lat1Function, PhysicalFunction timestamp1Function, PhysicalFunction staticGeometryFunction)
    : isTemporal6Param(false)
{
    parameterFunctions.reserve(4);
    parameterFunctions.push_back(std::move(lon1Function));
    parameterFunctions.push_back(std::move(lat1Function));
    parameterFunctions.push_back(std::move(timestamp1Function));
    parameterFunctions.push_back(std::move(staticGeometryFunction));
}

// Constructor with 6 parameters for temporal-temporal intersection
ExternBoolContainsSpansetTimestamptzPhysicalFunction::ExternBoolContainsSpansetTimestamptzPhysicalFunction(PhysicalFunction lon1Function, PhysicalFunction lat1Function, PhysicalFunction timestamp1Function, PhysicalFunction lon2Function, PhysicalFunction lat2Function, PhysicalFunction timestamp2Function)
    : isTemporal6Param(true)
{
    parameterFunctions.reserve(6);
    parameterFunctions.push_back(std::move(lon1Function));
    parameterFunctions.push_back(std::move(lat1Function));
    parameterFunctions.push_back(std::move(timestamp1Function));
    parameterFunctions.push_back(std::move(lon2Function));
    parameterFunctions.push_back(std::move(lat2Function));
    parameterFunctions.push_back(std::move(timestamp2Function));
}

VarVal ExternBoolContainsSpansetTimestamptzPhysicalFunction::execute(const Record& record, ArenaRef& arena) const
{
    std::cout << "ExternBoolContainsSpansetTimestamptzPhysicalFunction::execute called with " << parameterFunctions.size() << " arguments" << std::endl;
    
    // Execute all parameter functions to get their values
    std::vector<VarVal> parameterValues;
    parameterValues.reserve(parameterFunctions.size());
    for (const auto& paramFunc : parameterFunctions) {
        parameterValues.push_back(paramFunc.execute(record, arena));
    }
    
    if (isTemporal6Param) {
        // 6-parameter case: temporal-temporal intersection
        return executeTemporal6Param(parameterValues);
    } else {
        // 4-parameter case: temporal-static intersection
        return executeTemporal4Param(parameterValues);
    }
}

VarVal ExternBoolContainsSpansetTimestamptzPhysicalFunction::executeTemporal6Param(const std::vector<VarVal>& params) const
{
    // Extract coordinate values: lon1, lat1, timestamp1, lon2, lat2, timestamp2
    auto lon1 = params[0].cast<nautilus::val<double>>();
    auto lat1 = params[1].cast<nautilus::val<double>>();
    auto timestamp1 = params[2].cast<nautilus::val<uint64_t>>();
    auto lon2 = params[3].cast<nautilus::val<double>>();
    auto lat2 = params[4].cast<nautilus::val<double>>();
    auto timestamp2 = params[5].cast<nautilus::val<uint64_t>>();
    
    std::cout << "6-param temporal-temporal externBoolContainsSpansetTimestamptz with coordinate values" << std::endl;
    
    // Use nautilus::invoke to call external MEOS function with coordinate parameters
    const auto result = nautilus::invoke(
        +[](double lon1_val, double lat1_val, uint64_t ts1_val, double lon2_val, double lat2_val, uint64_t ts2_val) -> void {
            try {
                // Use the existing global MEOS initialization mechanism
                MEOS::Meos::ensureMeosInitialized();
                
                // Convert UINT64 timestamps to MEOS timestamp strings
                std::string timestamp1_str = MEOS::Meos::convertSecondsToTimestamp(ts1_val);
                std::string timestamp2_str = MEOS::Meos::convertSecondsToTimestamp(ts2_val);
                
                // Build temporal geometry WKT strings from coordinates and timestamps
                std::string left_geometry_wkt = fmt::format("SRID=4326;POINT({} {})@{}", lon1_val, lat1_val, timestamp1_str);
                std::string right_geometry_wkt = fmt::format("SRID=4326;POINT({} {})@{}", lon2_val, lat2_val, timestamp2_str);
                
                std::cout << "Built temporal geometries:" << std::endl;
                std::cout << "Left: " << left_geometry_wkt << std::endl;
                std::cout << "Right: " << right_geometry_wkt << std::endl;
                
                // Both geometries are temporal points, use temporal-temporal externBoolContainsSpansetTimestamptz
                std::cout << "Using temporal-temporal externBoolContainsSpansetTimestamptz (extern bool contains_spanset_timestamptz)" << std::endl;
                MEOS::Meos::TemporalGeometry left_temporal(left_geometry_wkt);
                MEOS::Meos::TemporalGeometry right_temporal(right_geometry_wkt);
                void function_result = left_temporal.externBoolContainsSpansetTimestamptz(right_temporal);
                
                std::cout << "ExternBoolContainsSpansetTimestamptz result: " << function_result << std::endl;
                return function_result;
            } catch (const std::exception& e) {
                std::cerr << "Error in externBoolContainsSpansetTimestamptz temporal-temporal: " << e.what() << std::endl;
                return -1; // Error indicator
            }
        },
        lon1, lat1, timestamp1, lon2, lat2, timestamp2
    );
    
    return VarVal(result);
}

VarVal ExternBoolContainsSpansetTimestamptzPhysicalFunction::executeTemporal4Param(const std::vector<VarVal>& params) const
{
    // Extract coordinate values and static geometry: lon, lat, timestamp, static_geometry_wkt
    auto lon = params[0].cast<nautilus::val<double>>();
    auto lat = params[1].cast<nautilus::val<double>>();
    auto timestamp = params[2].cast<nautilus::val<uint64_t>>();
    auto static_geometry_data = params[3].cast<nautilus::val<VariableSizedData>>();
    
    std::cout << "4-param temporal-static externBoolContainsSpansetTimestamptz with coordinate and static geometry" << std::endl;
    
    // Use nautilus::invoke to call external MEOS function
    const auto result = nautilus::invoke(
        +[](double lon_val, double lat_val, uint64_t timestamp_val, VariableSizedData static_geom_data) -> void {
            try {
                // Use the existing global MEOS initialization mechanism
                MEOS::Meos::ensureMeosInitialized();
                
                // Convert UINT64 timestamp to MEOS timestamp string
                std::string timestamp_str = MEOS::Meos::convertSecondsToTimestamp(timestamp_val);
                
                // Build temporal geometry WKT string from coordinates and timestamp
                std::string temporal_geometry_wkt = fmt::format("SRID=4326;POINT({} {})@{}", lon_val, lat_val, timestamp_str);
                
                // Extract static geometry WKT string from VariableSizedData
                std::string static_geometry_wkt(static_geom_data.data, static_geom_data.size);
                
                std::cout << "Built temporal geometry: " << temporal_geometry_wkt << std::endl;
                std::cout << "Static geometry: " << static_geometry_wkt << std::endl;
                
                // Use temporal-static externBoolContainsSpansetTimestamptz
                std::cout << "Using temporal-static externBoolContainsSpansetTimestamptz (extern bool contains_spanset_timestamptz)" << std::endl;
                MEOS::Meos::TemporalGeometry temporal_geom(temporal_geometry_wkt);
                MEOS::Meos::StaticGeometry static_geom(static_geometry_wkt);
                void function_result = temporal_geom.externBoolContainsSpansetTimestamptzStatic(static_geom);
                
                std::cout << "ExternBoolContainsSpansetTimestamptz result: " << function_result << std::endl;
                return function_result;
            } catch (const std::exception& e) {
                std::cerr << "Error in externBoolContainsSpansetTimestamptz temporal-static: " << e.what() << std::endl;
                return -1; // Error indicator
            }
        },
        lon, lat, timestamp, static_geometry_data
    );
    
    return VarVal(result);
}

PhysicalFunctionRegistryReturnType PhysicalFunctionGeneratedRegistrar::RegisterExternBoolContainsSpansetTimestamptzPhysicalFunction(PhysicalFunctionRegistryArguments arguments)
{
    if (arguments.children.size() == 4) {
        return ExternBoolContainsSpansetTimestamptzPhysicalFunction(arguments.children[0], arguments.children[1], arguments.children[2], arguments.children[3]);
    } else if (arguments.children.size() == 6) {
        return ExternBoolContainsSpansetTimestamptzPhysicalFunction(arguments.children[0], arguments.children[1], arguments.children[2], arguments.children[3], arguments.children[4], arguments.children[5]);
    } else {
        PRECONDITION(false, "ExternBoolContainsSpansetTimestamptzPhysicalFunction requires 4 or 6 children, but got {}", arguments.children.size());
    }
}

}