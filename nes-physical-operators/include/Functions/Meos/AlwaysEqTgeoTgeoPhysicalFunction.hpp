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

#pragma once

#include <Functions/PhysicalFunction.hpp>
#include <Nautilus/DataTypes/VarVal.hpp>
#include <Nautilus/Interface/Record.hpp>
#include <memory>
#include <vector>

namespace NES {

/**
 * @brief Physical function for MEOS always_eq_tgeo_tgeo operation
 * 
 * This function executes the meos spatial relationship using MEOS always_eq_tgeo_tgeo
 * using the MEOS library function always_eq_tgeo_tgeo.
 * 
 * Function signature: extern int always_eq_tgeo_tgeo(const Temporal *temp1, const Temporal *temp2);
 */
class AlwaysEqTgeoTgeoPhysicalFunction : public PhysicalFunctionConcept {
public:
    /**
     * @brief Constructor for always_eq_tgeo_tgeo physical function
     * @param param0 const Temporal * parameter
     * @param param1 const Temporal * parameter

     */
    AlwaysEqTgeoTgeoPhysicalFunction(        PhysicalFunction param0Function,
        PhysicalFunction param1Function);

    /**
     * @brief Execute the MEOS function with given parameters
     * @param record Input record containing data
     * @param arena Memory arena for allocations
     * @return Result of the MEOS function execution
     */
    VarVal execute(const Record& record, ArenaRef& arena) const override;

private:
    std::vector<PhysicalFunction> parameterFunctions;  ///< Parameter functions to execute
};

} // namespace NES