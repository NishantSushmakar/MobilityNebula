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

#include <memory>
#include <set>
#include <unordered_map>
#include <utility>
#include <Identifiers/Identifiers.hpp>
#include <Plans/LogicalPlan.hpp>
#include <Sources/SourceCatalog.hpp>


namespace NES::LegacyOptimizer
{

/**
 * @brief This rule injects a LogicalSequenceOperator in front of a window aggregation if one of the aggregation requires sequential processing
 */
class SequentialAggregationRule
{
public:
    void apply(LogicalPlan& queryPlan) const;
};
}
