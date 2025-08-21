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

#include <Functions/Meos/AdjacentTspatialStboxPhysicalFunction.hpp>
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

AdjacentTspatialStboxPhysicalFunction::AdjacentTspatialStboxPhysicalFunction(        PhysicalFunction param0Function,
        PhysicalFunction param1Function)
{
    parameterFunctions.reserve(2);
    parameterFunctions.push_back(std::move(param0Function));
    parameterFunctions.push_back(std::move(param1Function));

}

VarVal AdjacentTspatialStboxPhysicalFunction::execute(const Record& record, ArenaRef& arena) const
{
    
    // Execute all parameter functions to get their values
    std::vector<VarVal> parameterValues;
    parameterValues.reserve(2);
    for (const auto& paramFunc : parameterFunctions) {
        parameterValues.push_back(paramFunc.execute(record, arena));
    }
    
    
    auto param0_varsized = parameterValues[0].cast<VariableSizedData>();
    auto param1_varsized = parameterValues[1].cast<VariableSizedData>();

    
    
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
                
                MEOS::Temporal* param0_obj = MEOS::tgeometry_in(param0_str.c_str());
                if (!param0_obj) {
                    return -1; // Failed to parse parameter
                }

                std::string param1_str(param1_ptr, param1_size);
                
                // Strip outer quotes if present
                if (param1_str.length() >= 2 && param1_str.front() == '\'' && param1_str.back() == '\'') {
                    param1_str = param1_str.substr(1, param1_str.length() - 2);
                }
                
                MEOS::STBox* param1_obj = MEOS::stbox_in(param1_str.c_str());
                if (!param1_obj) {
                    return -1; // Failed to parse parameter
                }


                
                // Call the direct MEOS function
                auto meos_result = MEOS::adjacent_tspatial_stbox(param0_obj, param1_obj);
                
                                // Cleanup const Temporal * object
                if (param0_obj) free(param0_obj);
                // Cleanup const STBox * object
                if (param1_obj) free(param1_obj);

                
                return static_cast<int32_t>(meos_result);
            } catch (const std::exception& e) {
                // MEOS exception occurred
                return static_cast<int32_t>(-1);  // Error case
            } catch (...) {
                // Unknown error occurred
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
PhysicalFunctionGeneratedRegistrar::RegisterAdjacentTspatialStboxPhysicalFunction(PhysicalFunctionRegistryArguments physicalFunctionRegistryArguments)
{
    PRECONDITION(physicalFunctionRegistryArguments.childFunctions.size() == 2, "AdjacentTspatialStboxPhysicalFunction requires exactly 2 child functions, but got {}", physicalFunctionRegistryArguments.childFunctions.size());
    return AdjacentTspatialStboxPhysicalFunction(physicalFunctionRegistryArguments.childFunctions[0], physicalFunctionRegistryArguments.childFunctions[1]);
}

} // namespace NES