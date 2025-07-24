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

#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <DataTypes/DataType.hpp>
#include <DataTypes/DataTypeProvider.hpp>
#include <DataTypes/Schema.hpp>
#include <Functions/Meos/TemporalIntersectsGeometryLogicalFunction.hpp>
#include <Functions/LogicalFunction.hpp>
#include <Serialization/DataTypeSerializationUtil.hpp>
#include <Util/PlanRenderer.hpp>
#include <fmt/format.h>
#include <ErrorHandling.hpp>
#include <LogicalFunctionRegistry.hpp>
#include <SerializableVariantDescriptor.pb.h>

namespace NES
{

TemporalIntersectsGeometryLogicalFunction::TemporalIntersectsGeometryLogicalFunction(LogicalFunction geometryString)
    : dataType(DataTypeProvider::provideDataType(DataType::Type::BOOLEAN))
    , geometryString(std::move(geometryString))
{
}

DataType TemporalIntersectsGeometryLogicalFunction::getDataType() const
{
    return dataType;
};

LogicalFunction TemporalIntersectsGeometryLogicalFunction::withDataType(const DataType& dataType) const
{
    auto copy = *this;
    copy.dataType = dataType;
    return copy;
};

std::vector<LogicalFunction> TemporalIntersectsGeometryLogicalFunction::getChildren() const
{
    return {geometryString};
};

LogicalFunction TemporalIntersectsGeometryLogicalFunction::withChildren(const std::vector<LogicalFunction>& children) const
{
    PRECONDITION(children.size() == 1, "TemporalIntersectsGeometryLogicalFunction requires exactly one child, but got {}", children.size());
    auto copy = *this;
    copy.geometryString = children[0];
    return copy;
};

std::string_view TemporalIntersectsGeometryLogicalFunction::getType() const
{
    return NAME;
}

bool TemporalIntersectsGeometryLogicalFunction::operator==(const LogicalFunctionConcept& rhs) const
{
    if (const auto* other = dynamic_cast<const TemporalIntersectsGeometryLogicalFunction*>(&rhs))
    {
        return geometryString == other->geometryString;
    }
    return false;
}

std::string TemporalIntersectsGeometryLogicalFunction::explain(ExplainVerbosity verbosity) const
{
    return fmt::format("TEMPORAL_INTERSECTS_GEOMETRY({})", geometryString.explain(verbosity));
}

LogicalFunction TemporalIntersectsGeometryLogicalFunction::withInferredDataType(const Schema& schema) const
{
    std::vector<LogicalFunction> newChildren;
    for (auto& node : getChildren())
    {
        newChildren.push_back(node.withInferredDataType(schema));
    }
    
    /// Check if child dataType is correct - temporal geometry should be VARSIZED (string)
    INVARIANT(
        newChildren[0].getDataType().isType(DataType::Type::VARSIZED), 
        "TemporalIntersectsGeometry requires VARSIZED input for WKT temporal geometry, but was: {}", 
        newChildren[0].getDataType());
        
    return this->withChildren(newChildren);
}

SerializableFunction TemporalIntersectsGeometryLogicalFunction::serialize() const
{
    SerializableFunction serializedFunction;
    serializedFunction.set_function_type(NAME);
    serializedFunction.add_children()->CopyFrom(geometryString.serialize());
    DataTypeSerializationUtil::serializeDataType(this->getDataType(), serializedFunction.mutable_data_type());
    return serializedFunction;
}

LogicalFunctionRegistryReturnType LogicalFunctionGeneratedRegistrar::RegisterTemporalIntersectsGeometryLogicalFunction(LogicalFunctionRegistryArguments arguments)
{
    PRECONDITION(arguments.children.size() == 1, "TemporalIntersectsGeometryLogicalFunction requires exactly one child, but got {}", arguments.children.size());
    return TemporalIntersectsGeometryLogicalFunction(arguments.children[0]);
}

}