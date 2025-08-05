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

#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <DataTypes/DataType.hpp>
#include <DataTypes/DataTypeProvider.hpp>
#include <DataTypes/Schema.hpp>
#include <Functions/Meos/ExternBoolContainsSpanIntLogicalFunction.hpp>
#include <Functions/LogicalFunction.hpp>
#include <Serialization/DataTypeSerializationUtil.hpp>
#include <Util/PlanRenderer.hpp>
#include <fmt/format.h>
#include <ErrorHandling.hpp>
#include <LogicalFunctionRegistry.hpp>
#include <SerializableVariantDescriptor.pb.h>

namespace NES
{

// 4-parameter constructor for temporal-static intersection
ExternBoolContainsSpanIntLogicalFunction::ExternBoolContainsSpanIntLogicalFunction(LogicalFunction lon1, LogicalFunction lat1, LogicalFunction timestamp1, LogicalFunction staticGeometry)
    : dataType(DataTypeProvider::provideDataType(DataType::Type::INT32))
    , isTemporal6Param(false)
{
    parameters.reserve(4);
    parameters.push_back(std::move(lon1));
    parameters.push_back(std::move(lat1));
    parameters.push_back(std::move(timestamp1));
    parameters.push_back(std::move(staticGeometry));
}

// 6-parameter constructor for temporal-temporal intersection
ExternBoolContainsSpanIntLogicalFunction::ExternBoolContainsSpanIntLogicalFunction(LogicalFunction lon1, LogicalFunction lat1, LogicalFunction timestamp1, LogicalFunction lon2, LogicalFunction lat2, LogicalFunction timestamp2)
    : dataType(DataTypeProvider::provideDataType(DataType::Type::INT32))
    , isTemporal6Param(true)
{
    parameters.reserve(6);
    parameters.push_back(std::move(lon1));
    parameters.push_back(std::move(lat1));
    parameters.push_back(std::move(timestamp1));
    parameters.push_back(std::move(lon2));
    parameters.push_back(std::move(lat2));
    parameters.push_back(std::move(timestamp2));
}

DataType ExternBoolContainsSpanIntLogicalFunction::getDataType() const
{
    return dataType;
};

LogicalFunction ExternBoolContainsSpanIntLogicalFunction::withDataType(const DataType& dataType) const
{
    auto copy = *this;
    copy.dataType = dataType;
    return copy;
};

std::vector<LogicalFunction> ExternBoolContainsSpanIntLogicalFunction::getChildren() const
{
    return parameters;
};

LogicalFunction ExternBoolContainsSpanIntLogicalFunction::withChildren(const std::vector<LogicalFunction>& children) const
{
    PRECONDITION(children.size() == 4 || children.size() == 6, "ExternBoolContainsSpanIntLogicalFunction requires 4 or 6 children, but got {}", children.size());
    auto copy = *this;
    copy.parameters = children;
    copy.isTemporal6Param = (children.size() == 6);
    return copy;
};

std::string_view ExternBoolContainsSpanIntLogicalFunction::getType() const
{
    return NAME;
}

bool ExternBoolContainsSpanIntLogicalFunction::operator==(const LogicalFunctionConcept& rhs) const
{
    if (const auto* other = dynamic_cast<const ExternBoolContainsSpanIntLogicalFunction*>(&rhs))
    {
        return parameters == other->parameters && isTemporal6Param == other->isTemporal6Param;
    }
    return false;
}

std::string ExternBoolContainsSpanIntLogicalFunction::explain(ExplainVerbosity verbosity) const
{
    std::string args;
    for (size_t i = 0; i < parameters.size(); ++i) {
        if (i > 0) args += ", ";
        args += parameters[i].explain(verbosity);
    }
    return fmt::format("TEMPORAL_EXTERN BOOL CONTAINS_SPAN_INT_GEOMETRY({})", args);
}

LogicalFunction ExternBoolContainsSpanIntLogicalFunction::withInferredDataType(const Schema& schema) const
{
    std::vector<LogicalFunction> newChildren;
    for (auto& node : getChildren())
    {
        newChildren.push_back(node.withInferredDataType(schema));
    }
    
    if (isTemporal6Param) {
        // 6-parameter case: lon1, lat1, timestamp1, lon2, lat2, timestamp2
        INVARIANT(newChildren[0].getDataType().isNumeric(), "lon1 must be numeric, but was: {}", newChildren[0].getDataType());
        INVARIANT(newChildren[1].getDataType().isNumeric(), "lat1 must be numeric, but was: {}", newChildren[1].getDataType());
        INVARIANT(newChildren[2].getDataType().isType(DataType::Type::UINT64), "timestamp1 must be UINT64, but was: {}", newChildren[2].getDataType());
        INVARIANT(newChildren[3].getDataType().isNumeric(), "lon2 must be numeric, but was: {}", newChildren[3].getDataType());
        INVARIANT(newChildren[4].getDataType().isNumeric(), "lat2 must be numeric, but was: {}", newChildren[4].getDataType());
        INVARIANT(newChildren[5].getDataType().isType(DataType::Type::UINT64), "timestamp2 must be UINT64, but was: {}", newChildren[5].getDataType());
    } else {
        // 4-parameter case: lon1, lat1, timestamp1, static_geometry
        INVARIANT(newChildren[0].getDataType().isNumeric(), "lon1 must be numeric, but was: {}", newChildren[0].getDataType());
        INVARIANT(newChildren[1].getDataType().isNumeric(), "lat1 must be numeric, but was: {}", newChildren[1].getDataType());
        INVARIANT(newChildren[2].getDataType().isType(DataType::Type::UINT64), "timestamp1 must be UINT64, but was: {}", newChildren[2].getDataType());
        INVARIANT(newChildren[3].getDataType().isType(DataType::Type::VARSIZED), "static_geometry must be VARSIZED, but was: {}", newChildren[3].getDataType());
    }
        
    return this->withChildren(newChildren);
}

SerializableFunction ExternBoolContainsSpanIntLogicalFunction::serialize() const
{
    SerializableFunction serializedFunction;
    serializedFunction.set_function_type(NAME);
    for (const auto& param : parameters) {
        serializedFunction.add_children()->CopyFrom(param.serialize());
    }
    DataTypeSerializationUtil::serializeDataType(this->getDataType(), serializedFunction.mutable_data_type());
    return serializedFunction;
}

LogicalFunctionRegistryReturnType LogicalFunctionGeneratedRegistrar::RegisterExternBoolContainsSpanIntLogicalFunction(LogicalFunctionRegistryArguments arguments)
{
    if (arguments.children.size() == 4) {
        return ExternBoolContainsSpanIntLogicalFunction(arguments.children[0], arguments.children[1], arguments.children[2], arguments.children[3]);
    } else if (arguments.children.size() == 6) {
        return ExternBoolContainsSpanIntLogicalFunction(arguments.children[0], arguments.children[1], arguments.children[2], arguments.children[3], arguments.children[4], arguments.children[5]);
    } else {
        PRECONDITION(false, "ExternBoolContainsSpanIntLogicalFunction requires 4 or 6 children, but got {}", arguments.children.size());
    }
}

}