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

#include <Functions/Meos/OverbackTspatialTspatialLogicalFunction.hpp>
#include <DataTypes/DataTypeProvider.hpp>
#include <DataTypes/DataType.hpp>
#include <DataTypes/Schema.hpp>
#include <Functions/LogicalFunction.hpp>
#include <Serialization/DataTypeSerializationUtil.hpp>
#include <Util/PlanRenderer.hpp>
#include <fmt/format.h>
#include <ErrorHandling.hpp>
#include <LogicalFunctionRegistry.hpp>
#include <SerializableVariantDescriptor.pb.h>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace NES {

OverbackTspatialTspatialLogicalFunction::OverbackTspatialTspatialLogicalFunction(
        LogicalFunction param0,
        LogicalFunction param1
) : dataType(DataTypeProvider::provideDataType(DataType::Type::INT32)) {
    parameters.reserve(2);
    parameters.push_back(std::move(param0));
    parameters.push_back(std::move(param1));

}

DataType OverbackTspatialTspatialLogicalFunction::getDataType() const {
    return dataType;
}

LogicalFunction OverbackTspatialTspatialLogicalFunction::withDataType(const DataType& dataType) const {
    auto copy = *this;
    copy.dataType = dataType;
    return copy;
}

std::vector<LogicalFunction> OverbackTspatialTspatialLogicalFunction::getChildren() const {
    return parameters;
}

LogicalFunction OverbackTspatialTspatialLogicalFunction::withChildren(const std::vector<LogicalFunction>& children) const {
    PRECONDITION(children.size() == 2, "OverbackTspatialTspatialLogicalFunction requires 2 children, but got {}", children.size());
    auto copy = *this;
    copy.parameters = children;
    return copy;
}

std::string_view OverbackTspatialTspatialLogicalFunction::getType() const {
    return NAME;
}

bool OverbackTspatialTspatialLogicalFunction::operator==(const LogicalFunctionConcept& rhs) const {
    if (const auto* other = dynamic_cast<const OverbackTspatialTspatialLogicalFunction*>(&rhs)) {
        return parameters == other->parameters;
    }
    return false;
}

std::string OverbackTspatialTspatialLogicalFunction::explain(ExplainVerbosity verbosity) const {
    std::string args;
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) args += ", ";
        args += parameters[i].explain(verbosity);
    }
    return fmt::format("OVERBACK_TSPATIAL_TSPATIAL({})", args);
}

LogicalFunction OverbackTspatialTspatialLogicalFunction::withInferredDataType(const Schema& schema) const {
    std::vector<LogicalFunction> newChildren;
    for (auto& node : getChildren()) {
        newChildren.push_back(node.withInferredDataType(schema));
    }
    
    INVARIANT(newChildren[0].getDataType().isType(DataType::Type::VARSIZED), "param0 must be VARSIZED for Temporal type, but was: {}", newChildren[0].getDataType());
    INVARIANT(newChildren[1].getDataType().isType(DataType::Type::VARSIZED), "param1 must be VARSIZED for Temporal type, but was: {}", newChildren[1].getDataType());

        
    return this->withChildren(newChildren);
}

SerializableFunction OverbackTspatialTspatialLogicalFunction::serialize() const {
    SerializableFunction serializedFunction;
    serializedFunction.set_function_type(NAME);
    for (const auto& param : parameters) {
        serializedFunction.add_children()->CopyFrom(param.serialize());
    }
    DataTypeSerializationUtil::serializeDataType(this->getDataType(), serializedFunction.mutable_data_type());
    return serializedFunction;
}

LogicalFunctionRegistryReturnType LogicalFunctionGeneratedRegistrar::RegisterOverbackTspatialTspatialLogicalFunction(LogicalFunctionRegistryArguments arguments) {
    PRECONDITION(arguments.children.size() == 2, "OverbackTspatialTspatialLogicalFunction requires 2 children, but got {}", arguments.children.size());
    return OverbackTspatialTspatialLogicalFunction(arguments.children[0], arguments.children[1]);
}

} // namespace NES