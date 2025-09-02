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

#include <Functions/Meos/EtouchesTpointGeoPhysicalFunction.hpp>
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

EtouchesTpointGeoPhysicalFunction::EtouchesTpointGeoPhysicalFunction(        PhysicalFunction param0Function,
        PhysicalFunction param1Function)
{
    parameterFunctions.reserve(2);
    parameterFunctions.push_back(std::move(param0Function));
    parameterFunctions.push_back(std::move(param1Function));

}

VarVal EtouchesTpointGeoPhysicalFunction::execute(const Record& record, ArenaRef& arena) const
{
    printf("EtouchesTpointGeoPhysicalFunction::execute called with 2 parameters\n");
    
    // Execute all parameter functions to get their values
    std::vector<VarVal> parameterValues;
    parameterValues.reserve(2);
    for (const auto& paramFunc : parameterFunctions) {
        parameterValues.push_back(paramFunc.execute(record, arena));
    }
    
    printf("DEBUG: Extracted %zu parameters\n", parameterValues.size());
    
    auto param0_varsized = parameterValues[0].cast<VariableSizedData>();
    auto param1_varsized = parameterValues[1].cast<VariableSizedData>();

    
    printf("EtouchesTpointGeo with extracted parameter values\n");
    
    // Use nautilus::invoke to call direct MEOS function
    const auto result = nautilus::invoke(
        +[](const char* param0_ptr, uint32_t param0_size, const char* param1_ptr, uint32_t param1_size) -> int32_t {
            try {
                // Use the existing global MEOS initialization mechanism
                MEOS::Meos::ensureMeosInitialized();
                
                                // MEOS Type Conversions
                std::string param0_str(param0_ptr, param0_size);
                
                // Strip outer quotes if present
                if (param0_str.length() >= 2 && param0_str.front() == '\'' && param0_str.back() == '\'') {
                    param0_str = param0_str.substr(1, param0_str.length() - 2);
                }
                
                MEOS::Temporal* param0_obj = MEOS::tgeompoint_in(param0_str.c_str());
                if (!param0_obj) {
                    return -1; // Failed to parse parameter
                }

                std::string param1_str(param1_ptr, param1_size);
                
                // Strip outer quotes if present
                if (param1_str.length() >= 2 && param1_str.front() == '\'' && param1_str.back() == '\'') {
                    param1_str = param1_str.substr(1, param1_str.length() - 2);
                }
                
                MEOS::GSERIALIZED* param1_obj = MEOS::geom_in(param1_str.c_str(), -1);
                if (!param1_obj) {
                    return -1; // Failed to parse parameter
                }


                
                // Call the direct MEOS function
                printf("DEBUG: Calling MEOS::etouches_tpoint_geo...\n");
                auto meos_result = MEOS::etouches_tpoint_geo(param0_obj, param1_obj);
                printf("DEBUG: MEOS etouches_tpoint_geo result: %d\n", meos_result);
                
                                // Cleanup const Temporal * object
                if (param0_obj) free(param0_obj);
                // Cleanup const GSERIALIZED * object
                if (param1_obj) free(param1_obj);

                
                return static_cast<int32_t>(meos_result);
            } catch (const std::exception& e) {
                printf("MEOS exception in etouches_tpoint_geo: %s\n", e.what());
                return static_cast<int32_t>(-1);  // Error case
            } catch (...) {
                printf("Unknown error in etouches_tpoint_geo\n");
                return static_cast<int32_t>(-1);  // Error case
            }
        },
        param0_varsized.getContent(), param0_varsized.getContentSize(),
        param1_varsized.getContent(), param1_varsized.getContentSize()
    );
    
    // Convert result to NES VarVal type
    return VarVal(result);
}

// Conversion method implementations will be added based on parameter types

PhysicalFunctionRegistryReturnType
PhysicalFunctionGeneratedRegistrar::RegisterEtouchesTpointGeoPhysicalFunction(PhysicalFunctionRegistryArguments physicalFunctionRegistryArguments)
{
    PRECONDITION(physicalFunctionRegistryArguments.childFunctions.size() == 2, "EtouchesTpointGeoPhysicalFunction requires exactly 2 child functions, but got {}", physicalFunctionRegistryArguments.childFunctions.size());
    return EtouchesTpointGeoPhysicalFunction(physicalFunctionRegistryArguments.childFunctions[0], physicalFunctionRegistryArguments.childFunctions[1]);
}

} // namespace NES