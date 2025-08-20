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

#include <Functions/LogicalFunction.hpp>
#include <string_view>
#include <utility>
#include <vector>

namespace NES {

/**
 * @brief MEOS ecovers_tgeo_tgeo logical function
 * 
 * This function implements the ecovers_tgeo_tgeo operation from MEOS library.
 * Function signature: extern int ecovers_tgeo_tgeo(const Temporal *temp1, const Temporal *temp2);
 */
class EcoversTgeoTgeoLogicalFunction : public LogicalFunctionConcept {
public:
    static constexpr std::string_view NAME = "EcoversTgeoTgeo";

    /**
     * @brief Constructor for EcoversTgeoTgeoLogicalFunction
     */
    EcoversTgeoTgeoLogicalFunction(
        LogicalFunction param0,
        LogicalFunction param1
    );

    /// Get the data type of this function
    DataType getDataType() const override;

    /// Create a new function with the specified data type
    LogicalFunction withDataType(const DataType& dataType) const override;

    /// Get child functions
    std::vector<LogicalFunction> getChildren() const override;

    /// Create a new function with different children
    LogicalFunction withChildren(const std::vector<LogicalFunction>& children) const override;

    /// Get the type name of this function
    std::string_view getType() const override;

    /// Equality comparison
    bool operator==(const LogicalFunctionConcept& rhs) const override;

    /// Generate explanation string
    std::string explain(ExplainVerbosity verbosity) const override;

    /// Infer data type from schema
    LogicalFunction withInferredDataType(const Schema& schema) const override;

    /// Serialize this function
    SerializableFunction serialize() const override;

private:
    DataType dataType;
    std::vector<LogicalFunction> parameters;
};

} // namespace NES