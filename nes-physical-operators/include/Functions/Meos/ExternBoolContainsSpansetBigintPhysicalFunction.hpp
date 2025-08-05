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

#pragma once

#include <Functions/PhysicalFunction.hpp>
#include <Nautilus/DataTypes/VarVal.hpp>
#include <Nautilus/Interface/Record.hpp>

namespace NES {

class ExternBoolContainsSpansetBigintPhysicalFunction : public PhysicalFunctionConcept {
public:
    /// Constructor with 4 parameters for temporal-static intersection: lon1, lat1, timestamp1, static_geometry_wkt
    ExternBoolContainsSpansetBigintPhysicalFunction(PhysicalFunction lon1Function, PhysicalFunction lat1Function, PhysicalFunction timestamp1Function, PhysicalFunction staticGeometryFunction);
    
    /// Constructor with 6 parameters for temporal-temporal intersection: lon1, lat1, timestamp1, lon2, lat2, timestamp2
    ExternBoolContainsSpansetBigintPhysicalFunction(PhysicalFunction lon1Function, PhysicalFunction lat1Function, PhysicalFunction timestamp1Function, PhysicalFunction lon2Function, PhysicalFunction lat2Function, PhysicalFunction timestamp2Function);

    /// Execute the function with the given record and arena
    VarVal execute(const Record& record, ArenaRef& arena) const override;

private:
    std::vector<PhysicalFunction> parameterFunctions;  // Stores 4 or 6 parameter functions
    bool isTemporal6Param;  // true for 6-param temporal-temporal, false for 4-param temporal-static
    
    // Helper methods for different parameter cases
    VarVal executeTemporal6Param(const std::vector<VarVal>& params) const;
    VarVal executeTemporal4Param(const std::vector<VarVal>& params) const;
};

}