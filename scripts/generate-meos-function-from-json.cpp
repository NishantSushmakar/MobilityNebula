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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <sstream>

/**
 * @brief MEOS Function Generator from JSON Configuration
 * 
 * This tool reads meos-functions.json and generates complete MEOS function
 * implementations using templates.
 */
class MEOSFunctionGenerator {
private:
    struct FunctionInfo {
        std::string name;
        std::string return_type;
        std::vector<std::string> param_types;
        std::string function_family;
        std::string logical_class;
        std::string physical_class;
        std::string sql_token;
        std::string signature;
        int param_count;
    };
    
    std::map<std::string, FunctionInfo> functions;
    
    std::string readTemplate(const std::string& templatePath) {
        std::ifstream file(templatePath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open template " << templatePath << std::endl;
            return "";
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        return content;
    }
    
    std::string replaceTemplatePlaceholders(const std::string& templateContent, const FunctionInfo& func) {
        std::string result = templateContent;
        
        // Basic replacements
        result = std::regex_replace(result, std::regex(R"(\{\{MEOS_FUNCTION_NAME\}\})"), func.name);
        result = std::regex_replace(result, std::regex(R"(\{\{CLASS_NAME\}\})"), func.logical_class.substr(0, func.logical_class.find("LogicalFunction")));
        result = std::regex_replace(result, std::regex(R"(\{\{MEOS_SIGNATURE\}\})"), func.signature);
        result = std::regex_replace(result, std::regex(R"(\{\{PARAM_COUNT\}\})"), std::to_string(func.param_count));
        result = std::regex_replace(result, std::regex(R"(\{\{NES_RETURN_TYPE\}\})"), mapMEOSTypeToNES(func.return_type));
        result = std::regex_replace(result, std::regex(R"(\{\{MEOS_RETURN_TYPE\}\})"), func.return_type);
        
        // Generate function description
        std::string description = generateFunctionDescription(func);
        result = std::regex_replace(result, std::regex(R"(\{\{FUNCTION_DESCRIPTION\}\})"), description);
        
        // Generate parameter-related content
        result = std::regex_replace(result, std::regex(R"(\{\{PARAMETER_DOCS\}\})"), generateParameterDocs(func));
        result = std::regex_replace(result, std::regex(R"(\{\{CONSTRUCTOR_PARAM_DOCS\}\})"), generateConstructorParamDocs(func));
        result = std::regex_replace(result, std::regex(R"(\{\{CONSTRUCTOR_PARAMS\}\})"), generateConstructorParams(func));
        result = std::regex_replace(result, std::regex(R"(\{\{PHYSICAL_CONSTRUCTOR_PARAMS\}\})"), generatePhysicalConstructorParams(func));
        result = std::regex_replace(result, std::regex(R"(\{\{VALIDATION_PARAMS\}\})"), generateValidationParams(func));
        result = std::regex_replace(result, std::regex(R"(\{\{PARAMETER_LIST\}\})"), generateParameterList(func));
        result = std::regex_replace(result, std::regex(R"(\{\{PARAMETER_NAMES\}\})"), generateParameterNames(func));
        result = std::regex_replace(result, std::regex(R"(\{\{CREATE_WITH_PARAMS\}\})"), generateCreateWithParams(func));
        result = std::regex_replace(result, std::regex(R"(\{\{EXPECTED_PARAM_TYPES\}\})"), generateExpectedParamTypes(func));
        result = std::regex_replace(result, std::regex(R"(\{\{PARAMETER_PUSH_BACKS\}\})"), generateParameterPushBacks(func));
        result = std::regex_replace(result, std::regex(R"(\{\{VALIDATION_LOGIC\}\})"), generateValidationLogic(func));
        result = std::regex_replace(result, std::regex(R"(\{\{RETURN_TYPE_DESCRIPTION\}\})"), generateReturnTypeDescription(func));
        result = std::regex_replace(result, std::regex(R"(\{\{UPPER_FUNCTION_NAME\}\})"), generateUpperFunctionName(func));
        result = std::regex_replace(result, std::regex(R"(\{\{TYPE_VALIDATION\}\})"), generateTypeValidation(func));
        result = std::regex_replace(result, std::regex(R"(\{\{REGISTRY_CONSTRUCTOR_ARGS\}\})"), generateRegistryConstructorArgs(func));
        result = std::regex_replace(result, std::regex(R"(\{\{PHYSICAL_REGISTRY_CONSTRUCTOR_ARGS\}\})"), generatePhysicalRegistryConstructorArgs(func));
        
        // Physical function specific
        result = std::regex_replace(result, std::regex(R"(\{\{EXECUTION_PARAMS\}\})"), generateExecutionParams(func));
        result = std::regex_replace(result, std::regex(R"(\{\{CONVERSION_METHODS\}\})"), generateConversionMethods(func));
        result = std::regex_replace(result, std::regex(R"(\{\{PARAMETER_CONVERSIONS\}\})"), generateParameterConversions(func));
        result = std::regex_replace(result, std::regex(R"(\{\{MEOS_FUNCTION_CALL\}\})"), generateMEOSFunctionCall(func));
        result = std::regex_replace(result, std::regex(R"(\{\{RESULT_CONVERSION\}\})"), generateResultConversion(func));
        result = std::regex_replace(result, std::regex(R"(\{\{CONVERSION_METHOD_IMPLEMENTATIONS\}\})"), generateConversionMethodImplementations(func));
        result = std::regex_replace(result, std::regex(R"(\{\{CONSTRUCTOR_ASSIGNMENTS\}\})"), generateConstructorAssignments(func));
        result = std::regex_replace(result, std::regex(R"(\{\{PARAMETER_EXTRACTIONS\}\})"), generateParameterExtractions(func));
        result = std::regex_replace(result, std::regex(R"(\{\{MEOS_PARAM_TYPES\}\})"), generateMeosParamTypes(func));
        result = std::regex_replace(result, std::regex(R"(\{\{MEOS_FUNCTION_ARGS\}\})"), generateMeosFunctionArgs(func));
        result = std::regex_replace(result, std::regex(R"(\{\{CONVERTED_RETURN_TYPE\}\})"), generateConvertedReturnType(func));
        result = std::regex_replace(result, std::regex(R"(\{\{MEOS_TYPE_CONVERSIONS\}\})"), generateMeosTypeConversions(func));
        result = std::regex_replace(result, std::regex(R"(\{\{MEOS_CLEANUP\}\})"), generateMeosCleanup(func));
        result = std::regex_replace(result, std::regex(R"(\{\{TEMPORAL_RETURN_HANDLING\}\})"), generateTemporalReturnHandling(func));
        result = std::regex_replace(result, std::regex(R"(\{\{RETURN_CONVERSION\}\})"), generateReturnConversion(func));
        result = std::regex_replace(result, std::regex(R"(\{\{ERROR_RETURN\}\})"), generateErrorReturn(func));
        result = std::regex_replace(result, std::regex(R"(\{\{RESULT_TO_VARVAL_CONVERSION\}\})"), generateResultToVarValConversion(func));
        result = std::regex_replace(result, std::regex(R"(\{\{SQL_TOKEN\}\})"), generateSqlToken(func));
        result = std::regex_replace(result, std::regex(R"(\{\{LOGICAL_CLASS\}\})"), func.logical_class);
        result = std::regex_replace(result, std::regex(R"(\{\{SQL_PARAMETER_EXTRACTIONS\}\})"), generateSqlParameterExtractions(func));
        
        return result;
    }
    
    std::string mapMEOSTypeToNES(const std::string& meosType) {
        // Boolean and integer types
        if (meosType == "int" || meosType == "bool") return "INT32";
        
        // Floating point types
        if (meosType == "double" || meosType == "float") return "FLOAT64";
        
        // MEOS pointer types - all become VARSIZED for string serialization
        if (meosType.find("Temporal*") != std::string::npos) return "VARSIZED";
        if (meosType.find("GSERIALIZED*") != std::string::npos) return "VARSIZED";
        if (meosType.find("TBOX*") != std::string::npos) return "VARSIZED";
        if (meosType.find("STBOX*") != std::string::npos) return "VARSIZED";
        if (meosType.find("Set*") != std::string::npos) return "VARSIZED";
        
        // Generic pointer types
        if (meosType.find("*") != std::string::npos) return "VARSIZED";
        
        return "INT32"; // Default fallback
    }
    
    std::string getMEOSTextInputConverter(const std::string& paramType) {
        // Temporal types - text input (use specific temporal geometry functions)
        if (paramType.find("Temporal") != std::string::npos) return "MEOS::tgeometry_in";
        if (paramType.find("TPoint") != std::string::npos) return "MEOS::tgeompoint_in"; // Specific for temporal points
        if (paramType.find("TInstant") != std::string::npos) return "MEOS::tgeompoint_in"; // Use tgeompoint for instants
        if (paramType.find("TSequence") != std::string::npos) return "MEOS::tgeometry_in";
        if (paramType.find("TSequenceSet") != std::string::npos) return "MEOS::tgeometry_in";
        
        // Geometry types - text input
        if (paramType.find("GSERIALIZED") != std::string::npos) return "MEOS::geom_in";
        
        // Box types - text input
        if (paramType.find("STBox") != std::string::npos) return "MEOS::stbox_in";
        if (paramType.find("TBox") != std::string::npos) return "MEOS::tbox_in";
        
        // Set and Span types - text input
        if (paramType.find("Set") != std::string::npos) return "MEOS::set_in";
        if (paramType.find("Span") != std::string::npos) return "MEOS::span_in";
        
        // Primitive types - no conversion needed, passed directly
        if (paramType == "bool" || paramType == "int" || paramType == "double" || paramType == "float") return "";
        
        return ""; // No conversion needed for other primitive types
    }

    
    std::string getMEOSTextOutputConverter(const std::string& returnType) {
        // Temporal types - text output
        if (returnType.find("Temporal") != std::string::npos) return "MEOS::temporal_out";
        if (returnType.find("TInstant") != std::string::npos) return "MEOS::tinstant_out";
        if (returnType.find("TSequence") != std::string::npos) return "MEOS::tsequence_out";
        if (returnType.find("TSequenceSet") != std::string::npos) return "MEOS::tsequenceset_out";
        
        // Geometry types - text output
        if (returnType.find("GSERIALIZED") != std::string::npos) return "MEOS::geo_out";
        
        // Box types - text output
        if (returnType.find("STBox") != std::string::npos) return "MEOS::stbox_out";
        if (returnType.find("TBox") != std::string::npos) return "MEOS::tbox_out";
        
        // Set and Span types - text output
        if (returnType.find("Set") != std::string::npos) return "MEOS::set_out";
        if (returnType.find("Span") != std::string::npos) return "MEOS::span_out";
        
        // Primitive types - text output
        if (returnType.find("bool") != std::string::npos) return "MEOS::bool_out";
        
        return ""; // No conversion needed for int, double, char*, etc.
    }

    
    std::string getConvertedReturnType(const std::string& returnType) {
        // All MEOS pointer types return as string (const char*) after conversion
        if (returnType.find("*") != std::string::npos) {
            return "const char*";
        }
        
        // Primitive types
        if (returnType == "int" || returnType == "bool") return "int32_t";
        if (returnType == "double" || returnType == "float") return "double";
        
        return "int32_t"; // default
    }
    
    std::string generateFunctionDescription(const FunctionInfo& func) {
        return "the " + func.function_family + " spatial relationship using MEOS " + func.name;
    }
    
    std::string generateParameterDocs(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            oss << " * - param" << i << ": " << func.param_types[i] << "\n";
        }
        return oss.str();
    }
    
    std::string generateConstructorParamDocs(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            oss << "     * @param param" << i << " " << func.param_types[i] << " parameter\n";
        }
        return oss.str();
    }
    
    std::string generateConstructorParams(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << "        LogicalFunction param" << i;
        }
        return oss.str();
    }
    
    std::string generatePhysicalConstructorParams(const FunctionInfo& func) {
        std::ostringstream oss;
        for (int i = 0; i < func.param_count; ++i) {
            if (i > 0) oss << ",\n";
            oss << "        PhysicalFunction param" << i << "Function";
        }
        return oss.str();
    }
    
    std::string generateValidationParams(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << "        const LogicalFunction& param" << i;
        }
        return oss.str();
    }
    
    std::string generateParameterList(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << "param" << i << "Function";
        }
        return oss.str();
    }
    
    std::string generateParameterNames(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << "param" << i;
        }
        return oss.str();
    }
    
    std::string generateCreateWithParams(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ",\n";
            oss << "        parameters[" << i << "]";
        }
        return oss.str();
    }

    std::string generateParameterPushBacks(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            oss << "    parameters.push_back(std::move(param" << i << "));\n";
        }
        return oss.str();
    }

    std::string generateUpperFunctionName(const FunctionInfo& func) {
        std::string upperName = func.name;
        std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);
        return upperName;
    }

    std::string generateTypeValidation(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            std::string paramType = func.param_types[i];
            if (paramType.find("Temporal") != std::string::npos) {
                oss << "    INVARIANT(newChildren[" << i << "].getDataType().isType(DataType::Type::VARSIZED), \"param" << i << " must be VARSIZED for Temporal type, but was: {}\", newChildren[" << i << "].getDataType());\n";
            } else if (paramType.find("GSERIALIZED") != std::string::npos) {
                oss << "    INVARIANT(newChildren[" << i << "].getDataType().isType(DataType::Type::VARSIZED), \"param" << i << " must be VARSIZED for geometry type, but was: {}\", newChildren[" << i << "].getDataType());\n";
            } else if (paramType.find("STBox") != std::string::npos || paramType.find("TBox") != std::string::npos) {
                oss << "    INVARIANT(newChildren[" << i << "].getDataType().isType(DataType::Type::VARSIZED), \"param" << i << " must be VARSIZED for box type, but was: {}\", newChildren[" << i << "].getDataType());\n";
            } else if (paramType.find("Set") != std::string::npos || paramType.find("Span") != std::string::npos) {
                oss << "    INVARIANT(newChildren[" << i << "].getDataType().isType(DataType::Type::VARSIZED), \"param" << i << " must be VARSIZED for set/span type, but was: {}\", newChildren[" << i << "].getDataType());\n";
            } else if (paramType.find("double") != std::string::npos || paramType.find("float") != std::string::npos) {
                oss << "    INVARIANT(newChildren[" << i << "].getDataType().isNumeric(), \"param" << i << " must be numeric, but was: {}\", newChildren[" << i << "].getDataType());\n";
            } else if (paramType.find("int") != std::string::npos || paramType.find("bool") != std::string::npos) {
                oss << "    INVARIANT(newChildren[" << i << "].getDataType().isNumeric() || newChildren[" << i << "].getDataType().isType(DataType::Type::INT32), \"param" << i << " must be numeric or INT32, but was: {}\", newChildren[" << i << "].getDataType());\n";
            }
        }
        return oss.str();
    }

    std::string generateRegistryConstructorArgs(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << "arguments.children[" << i << "]";
        }
        return oss.str();
    }

    std::string generatePhysicalRegistryConstructorArgs(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << "physicalFunctionRegistryArguments.childFunctions[" << i << "]";
        }
        return oss.str();
    }
    
    std::string generateExpectedParamTypes(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << "\"" << func.param_types[i] << "\"";
        }
        return oss.str();
    }
    
    std::string generateValidationLogic(const FunctionInfo& func) {
        return "    // Type-specific validation logic can be added here";
    }
    
    std::string generateReturnTypeDescription(const FunctionInfo& func) {
        if (func.return_type == "int") return "INT32 (0 = false, 1 = true)";
        if (func.return_type == "bool") return "BOOLEAN";
        if (func.return_type == "double") return "FLOAT64";
        return func.return_type;
    }
    
    std::string generateExecutionParams(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << "const void* param" << i;
        }
        return oss.str();
    }
    
    std::string generateConversionMethods(const FunctionInfo& func) {
        return "    // Parameter conversion methods will be generated based on types";
    }
    
    std::string generateParameterConversions(const FunctionInfo& func) {
        std::ostringstream oss;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            oss << "    " << func.param_types[i] << " converted_param" << i << " = ";
            oss << "static_cast<" << func.param_types[i] << ">(param" << i << ");\n";
        }
        return oss.str();
    }
    
    std::string generateMEOSFunctionCall(const FunctionInfo& func) {
        std::ostringstream oss;
        // Use MEOS namespace prefix to avoid conflicts
        oss << "MEOS::" << func.name << "(";
        for (int i = 0; i < func.param_count; ++i) {
            if (i > 0) oss << ", ";
            
            std::string converter = getMEOSTextInputConverter(func.param_types[i]);
            if (!converter.empty()) {
                // Use converted MEOS object
                oss << "param" << i << "_obj";
            } else {
                // Use primitive type directly
                oss << "param" << i;
            }
        }
        oss << ")";
        return oss.str();
    }
    
    std::string generateResultConversion(const FunctionInfo& func) {
        std::string outputConverter = getMEOSTextOutputConverter(func.return_type);
        
        if (!outputConverter.empty()) {
            // Complex MEOS type that needs text conversion
            if (func.return_type.find("TBox") != std::string::npos || func.return_type.find("STBox") != std::string::npos) {
                return outputConverter + "(result, 15)"; // 15 decimal places for boxes
            } else {
                return outputConverter + "(result)";
            }
        } else if (func.return_type == "int" || func.return_type == "bool") {
            return "static_cast<int32_t>(result)";
        } else if (func.return_type == "double" || func.return_type == "float") {
            return "static_cast<double>(result)";
        }
        
        return "result";
    }
    
    std::string generateConversionMethodImplementations(const FunctionInfo& func) {
        return "// Conversion method implementations will be added based on parameter types";
    }
    
    std::string generateConstructorAssignments(const FunctionInfo& func) {
        std::ostringstream oss;
        for (int i = 0; i < func.param_count; ++i) {
            oss << "    parameterFunctions.push_back(std::move(param" << i << "Function));\n";
        }
        return oss.str();
    }
    
    std::string generateParameterExtractions(const FunctionInfo& func) {
        std::ostringstream oss;
        for (int i = 0; i < func.param_count; ++i) {
            // Map parameter types to appropriate extraction
            if (func.param_types[i].find("Temporal") != std::string::npos) {
                oss << "    auto param" << i << "_varsized = parameterValues[" << i << "].cast<VariableSizedData>();\n";
            } else if (func.param_types[i].find("GSERIALIZED") != std::string::npos) {
                oss << "    auto param" << i << "_varsized = parameterValues[" << i << "].cast<VariableSizedData>();\n";
            } else if (func.param_types[i].find("STBox") != std::string::npos) {
                oss << "    auto param" << i << "_varsized = parameterValues[" << i << "].cast<VariableSizedData>();\n";
            } else if (func.param_types[i].find("TBox") != std::string::npos) {
                oss << "    auto param" << i << "_varsized = parameterValues[" << i << "].cast<VariableSizedData>();\n";
            } else if (func.param_types[i].find("Set") != std::string::npos) {
                oss << "    auto param" << i << "_varsized = parameterValues[" << i << "].cast<VariableSizedData>();\n";
            } else if (func.param_types[i].find("Span") != std::string::npos) {
                oss << "    auto param" << i << "_varsized = parameterValues[" << i << "].cast<VariableSizedData>();\n";
            } else if (func.param_types[i].find("double") != std::string::npos) {
                oss << "    auto param" << i << " = parameterValues[" << i << "].cast<nautilus::val<double>>();\n";
            } else if (func.param_types[i].find("bool") != std::string::npos) {
                oss << "    auto param" << i << " = parameterValues[" << i << "].cast<nautilus::val<bool>>();\n";
            } else {
                oss << "    auto param" << i << " = parameterValues[" << i << "].cast<nautilus::val<int>>();\n";
            }
        }
        return oss.str();
    }
    
    std::string generateMeosParamTypes(const FunctionInfo& func) {
        std::ostringstream oss;
        bool needsComma = false;
        for (size_t i = 0; i < func.param_types.size(); ++i) {
            if (needsComma) oss << ", ";
            needsComma = true;
            
            // Map MEOS types to nautilus::invoke compatible types (raw data for conversion)
            if (func.param_types[i].find("Temporal") != std::string::npos || 
                func.param_types[i].find("GSERIALIZED") != std::string::npos ||
                func.param_types[i].find("STBox") != std::string::npos ||
                func.param_types[i].find("TBox") != std::string::npos ||
                func.param_types[i].find("Set") != std::string::npos ||
                func.param_types[i].find("Span") != std::string::npos) {
                oss << "const char* param" << i << "_ptr, uint32_t param" << i << "_size";
            } else if (func.param_types[i].find("double") != std::string::npos) {
                oss << "double param" << i;
            } else if (func.param_types[i].find("bool") != std::string::npos) {
                oss << "bool param" << i;
            } else {
                oss << "int param" << i;
            }
        }
        return oss.str();
    }
    
    std::string generateMeosFunctionArgs(const FunctionInfo& func) {
        std::ostringstream oss;
        for (int i = 0; i < func.param_count; ++i) {
            if (i > 0) oss << ",\n";
            // Map parameter types to appropriate nautilus::invoke arguments
            if (func.param_types[i].find("Temporal") != std::string::npos || 
                func.param_types[i].find("GSERIALIZED") != std::string::npos ||
                func.param_types[i].find("STBox") != std::string::npos ||
                func.param_types[i].find("TBox") != std::string::npos ||
                func.param_types[i].find("Set") != std::string::npos ||
                func.param_types[i].find("Span") != std::string::npos) {
                oss << "        param" << i << "_varsized.getContent(), param" << i << "_varsized.getContentSize()";
            } else {
                oss << "        param" << i;
            }
        }
        return oss.str();
    }
    
    std::string generateConvertedReturnType(const FunctionInfo& func) {
        if (func.return_type == "int" || func.return_type == "bool") {
            return "int32_t";
        } else if (func.return_type == "double") {
            return "double";
        } else if (func.return_type.find("Temporal") != std::string::npos) {
            return "const char*"; // Return C string pointer for temporal data
        }
        return "int32_t"; // default
    }
    
    std::string generateMeosTypeConversions(const FunctionInfo& func) {
        std::ostringstream oss;
        
        oss << "                // MEOS Type Conversions\n";
        
        for (int i = 0; i < func.param_count; ++i) {
            // Check if this is a primitive type that doesn't need string parsing
            if (func.param_types[i] == "bool" || func.param_types[i] == "int" || func.param_types[i] == "double") {
                // Primitive types are passed directly, no conversion needed
                continue;
            }
            
            std::string textConverter = getMEOSTextInputConverter(func.param_types[i]);
            
            if (!textConverter.empty()) {
                // Text conversion for parameter
                oss << "                std::string param" << i << "_str(param" << i << "_ptr, param" << i << "_size);\n";
                oss << "                \n";
                oss << "                // Strip outer quotes if present\n";
                oss << "                if (param" << i << "_str.length() >= 2 && param" << i << "_str.front() == '\\'' && param" << i << "_str.back() == '\\'') {\n";
                oss << "                    param" << i << "_str = param" << i << "_str.substr(1, param" << i << "_str.length() - 2);\n";
                oss << "                }\n";
                oss << "                \n";
                
                // Generate text conversion based on type
                if (func.param_types[i].find("TPoint") != std::string::npos) {
                    oss << "                MEOS::Temporal* param" << i << "_obj = " << textConverter << "(param" << i << "_str.c_str());\n";
                } else if (func.param_types[i].find("Temporal") != std::string::npos) {
                    oss << "                MEOS::Temporal* param" << i << "_obj = " << textConverter << "(param" << i << "_str.c_str());\n";
                } else if (func.param_types[i].find("GSERIALIZED") != std::string::npos) {
                    oss << "                MEOS::GSERIALIZED* param" << i << "_obj = " << textConverter << "(param" << i << "_str.c_str(), -1);\n"; // -1 for typmod
                } else if (func.param_types[i].find("STBox") != std::string::npos) {
                    oss << "                MEOS::STBox* param" << i << "_obj = " << textConverter << "(param" << i << "_str.c_str());\n";
                } else if (func.param_types[i].find("Set") != std::string::npos) {
                    oss << "                MEOS::Set* param" << i << "_obj = " << textConverter << "(param" << i << "_str.c_str());\n";
                } else {
                    oss << "                auto param" << i << "_obj = " << textConverter << "(param" << i << "_str.c_str());\n";
                }
                
                oss << "                if (!param" << i << "_obj) {\n";
                if (func.return_type.find("Temporal") != std::string::npos) {
                    oss << "                    return \"ERROR: Failed to parse parameter " << i << "\";\n";
                } else {
                    oss << "                    return -1; // Failed to parse parameter\n";
                }
                oss << "                }\n\n";
            }
        }
        return oss.str();
    }
    
    std::string generateMeosCleanup(const FunctionInfo& func) {
        std::ostringstream oss;
        for (int i = 0; i < func.param_count; ++i) {
            // Skip primitive types - they don't need cleanup
            if (func.param_types[i] == "bool" || func.param_types[i] == "int" || func.param_types[i] == "double") {
                continue;
            }
            
            std::string converter = getMEOSTextInputConverter(func.param_types[i]);
            
            if (!converter.empty()) {
                // This is a MEOS object type that needs cleanup
                oss << "                // Cleanup " << func.param_types[i] << " object\n";
                oss << "                if (param" << i << "_obj) free(param" << i << "_obj);\n";
            }
        }
        return oss.str();
    }
    
    std::string generateTemporalReturnHandling(const FunctionInfo& func) {
        if (func.return_type.find("Temporal") != std::string::npos) {
            return R"(// Serialize Temporal* result to C string
                if (!meos_result) {
                    return "ERROR: Failed to compute result";
                }
                
                // Serialize temporal result
                char* temporal_str = MEOS::tgeo_out(meos_result, 15);
                if (!temporal_str) {
                    free(meos_result);
                    return "ERROR: Failed to serialize temporal result";
                }
                
                // Store result in static buffer (Nautilus-compatible approach)
                static thread_local char result_buffer[4096];
                strncpy(result_buffer, temporal_str, sizeof(result_buffer) - 1);
                result_buffer[sizeof(result_buffer) - 1] = '\0';
                
                // Cleanup MEOS allocated memory
                free(temporal_str);
                free(meos_result);
                
                return result_buffer;)";
        }
        return "";
    }
    
    std::string generateReturnConversion(const FunctionInfo& func) {
        if (func.return_type.find("Temporal") != std::string::npos) {
            return ""; // Already handled in temporal return handling
        } else {
            return "return static_cast<" + generateConvertedReturnType(func) + ">(meos_result);";
        }
    }
    
    std::string generateErrorReturn(const FunctionInfo& func) {
        if (func.return_type.find("Temporal") != std::string::npos) {
            return "return \"ERROR: Exception occurred\"; // Error case";
        } else {
            return "return static_cast<" + generateConvertedReturnType(func) + ">(-1);  // Error case";
        }
    }
    
    std::string generateResultToVarValConversion(const FunctionInfo& func) {
        if (func.return_type.find("Temporal") != std::string::npos) {
            return R"(// Convert C string result to VariableSizedData for temporal return
    size_t result_len = strlen(result);
    VariableSizedData resultData(result, result_len);
    return VarVal(resultData);)";
        } else {
            return "// Convert result to NES VarVal type\n    return VarVal(result);";
        }
    }

public:
    bool parseJSONConfig(const std::string& jsonPath) {
        std::ifstream file(jsonPath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << jsonPath << std::endl;
            return false;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();
        
        // Parse JSON using regex (simplified approach)
        std::regex functionRegex(
            "\"([^\"]+)\":\\s*\\{\\s*\"signature\":\\s*\"([^\"]+)\"[^}]*\"return_type\":\\s*\"([^\"]+)\"[^}]*\"param_count\":\\s*(\\d+)[^}]*\"param_types\":\\s*\\[([^\\]]*)\\][^}]*\"function_family\":\\s*\"([^\"]+)\"[^}]*\"logical_class\":\\s*\"([^\"]+)\"[^}]*\"physical_class\":\\s*\"([^\"]+)\"[^}]*\"sql_token\":\\s*\"([^\"]+)\""
        );
        
        std::sregex_iterator iter(content.begin(), content.end(), functionRegex);
        std::sregex_iterator end;
        
        for (; iter != end; ++iter) {
            const std::smatch& match = *iter;
            
            FunctionInfo func;
            func.name = match[1].str();
            func.signature = match[2].str();
            func.return_type = match[3].str();
            func.param_count = std::stoi(match[4].str());
            func.function_family = match[6].str();
            func.logical_class = match[7].str();
            func.physical_class = match[8].str();
            func.sql_token = match[9].str();
            
            // Parse parameter types
            std::string paramTypesStr = match[5].str();
            std::regex paramRegex("\"([^\"]+)\"");
            std::sregex_iterator paramIter(paramTypesStr.begin(), paramTypesStr.end(), paramRegex);
            std::sregex_iterator paramEnd;
            
            for (; paramIter != paramEnd; ++paramIter) {
                func.param_types.push_back(paramIter->str(1));
            }
            
            functions[func.name] = func;
        }
        
        std::cout << "Parsed " << functions.size() << " MEOS functions from JSON" << std::endl;
        return true;
    }
    
    bool generateFunction(const std::string& functionName, const std::string& outputDir) {
        auto it = functions.find(functionName);
        if (it == functions.end()) {
            std::cerr << "Function " << functionName << " not found in configuration" << std::endl;
            return false;
        }
        
        const FunctionInfo& func = it->second;
        return generateSingleFunction(func, outputDir);
    }
    
    bool generateAllFunctions(const std::string& outputDir) {
        std::cout << "Generating all " << functions.size() << " MEOS functions..." << std::endl;
        
        int successCount = 0;
        int failCount = 0;
        
        for (const auto& pair : functions) {
            const FunctionInfo& func = pair.second;
            std::cout << "Generating " << func.name << "..." << std::endl;
            
            if (generateSingleFunction(func, outputDir)) {
                successCount++;
            } else {
                failCount++;
                std::cerr << "Failed to generate " << func.name << std::endl;
            }
        }
        
        std::cout << "\nGeneration complete!" << std::endl;
        std::cout << "Successfully generated: " << successCount << " functions" << std::endl;
        if (failCount > 0) {
            std::cout << "Failed to generate: " << failCount << " functions" << std::endl;
        }
        
        return failCount == 0;
    }
    
    bool generateSQLParserUpdates(const std::string& grammarPath, const std::string& queryPlanCreatorPath) {
        std::cout << "Updating SQL parser files for all functions..." << std::endl;
        
        // Update ANTLR grammar file
        if (!updateAntlrGrammar(grammarPath)) {
            return false;
        }
        
        // Update query plan creator
        if (!updateQueryPlanCreator(queryPlanCreatorPath)) {
            return false;
        }
        
        std::cout << "SQL parser updates complete!" << std::endl;
        return true;
    }
    
    bool generateSQLParserUpdatesForFunction(const std::string& functionName, const std::string& grammarPath, const std::string& queryPlanCreatorPath) {
        auto it = functions.find(functionName);
        if (it == functions.end()) {
            std::cerr << "Function " << functionName << " not found for SQL parser updates" << std::endl;
            return false;
        }
        
        std::cout << "Updating SQL parser files for function: " << functionName << "..." << std::endl;
        
        // Update ANTLR grammar file with single function
        if (!updateAntlrGrammarForFunction(it->second, grammarPath)) {
            return false;
        }
        
        // Update query plan creator with single function
        if (!updateQueryPlanCreatorForFunction(it->second, queryPlanCreatorPath)) {
            return false;
        }
        
        // Update CMakeLists.txt files for both logical and physical operators
        if (!updateCMakeListsForFunction(it->second)) {
            std::cerr << "Warning: Failed to update CMakeLists.txt files" << std::endl;
        }
        
        std::cout << "SQL parser updates complete for " << functionName << "!" << std::endl;
        return true;
    }
    
    void listAvailableFunctions() {
        std::cout << "Available MEOS functions:\n";
        for (const auto& pair : functions) {
            const auto& func = pair.second;
            std::cout << "  " << func.name << " (" << func.param_count << " params, returns " << func.return_type << ")\n";
        }
    }
    
    bool updateCMakeListsForFunction(const FunctionInfo& func) {
        std::cout << "Updating CMakeLists.txt files for function: " << func.name << "..." << std::endl;
        
        // Update logical operators CMakeLists.txt
        std::string logicalCMakePath = "../nes-logical-operators/src/Functions/Meos/CMakeLists.txt";
        if (!updateLogicalCMakeLists(func, logicalCMakePath)) {
            std::cerr << "Failed to update logical operators CMakeLists.txt" << std::endl;
            return false;
        }
        
        // Update physical operators CMakeLists.txt
        std::string physicalCMakePath = "../nes-physical-operators/src/Functions/Meos/CMakeLists.txt";
        if (!updatePhysicalCMakeLists(func, physicalCMakePath)) {
            std::cerr << "Failed to update physical operators CMakeLists.txt" << std::endl;
            return false;
        }
        
        std::cout << "CMakeLists.txt files updated successfully for " << func.name << std::endl;
        return true;
    }
    
    bool updateCMakeListsForAllFunctions() {
        std::cout << "Updating CMakeLists.txt files for all functions..." << std::endl;
        
        // Update logical operators CMakeLists.txt
        std::string logicalCMakePath = "../nes-logical-operators/src/Functions/Meos/CMakeLists.txt";
        if (!updateLogicalCMakeListsForAll(logicalCMakePath)) {
            std::cerr << "Error: Failed to update logical operators CMakeLists.txt" << std::endl;
            return false;
        }
        
        // Update physical operators CMakeLists.txt
        std::string physicalCMakePath = "../nes-physical-operators/src/Functions/Meos/CMakeLists.txt";
        if (!updatePhysicalCMakeListsForAll(physicalCMakePath)) {
            std::cerr << "Error: Failed to update physical operators CMakeLists.txt" << std::endl;
            return false;
        }
        
        std::cout << "CMakeLists.txt files updated successfully for all functions" << std::endl;
        return true;
    }

private:
    bool generateSingleFunction(const FunctionInfo& func, const std::string& outputDir) {
        // Determine correct output directories
        std::string logicalDir = "../nes-logical-operators/include/Functions/Meos";
        std::string logicalSrcDir = "../nes-logical-operators/src/Functions/Meos";
        std::string physicalDir = "../nes-physical-operators/include/Functions/Meos";
        std::string physicalSrcDir = "../nes-physical-operators/src/Functions/Meos";
        
        // Generate logical function header
        std::string logicalHeaderTemplate = readTemplate("templates/MEOSLogicalFunction.hpp.template");
        if (logicalHeaderTemplate.empty()) return false;
        
        std::string logicalHeader = replaceTemplatePlaceholders(logicalHeaderTemplate, func);
        std::string logicalHeaderPath = logicalDir + "/" + func.logical_class + ".hpp";
        
        std::ofstream logicalHeaderFile(logicalHeaderPath);
        logicalHeaderFile << logicalHeader;
        logicalHeaderFile.close();
        
        // Generate logical function implementation
        std::string logicalImplTemplate = readTemplate("templates/MEOSLogicalFunction.cpp.template");
        if (logicalImplTemplate.empty()) return false;
        
        std::string logicalImpl = replaceTemplatePlaceholders(logicalImplTemplate, func);
        std::string logicalImplPath = logicalSrcDir + "/" + func.logical_class + ".cpp";
        
        std::ofstream logicalImplFile(logicalImplPath);
        logicalImplFile << logicalImpl;
        logicalImplFile.close();
        
        // Generate physical function header
        std::string physicalHeaderTemplate = readTemplate("templates/MEOSPhysicalFunction.hpp.template");
        if (physicalHeaderTemplate.empty()) return false;
        
        std::string physicalHeader = replaceTemplatePlaceholders(physicalHeaderTemplate, func);
        std::string physicalHeaderPath = physicalDir + "/" + func.physical_class + ".hpp";
        
        std::ofstream physicalHeaderFile(physicalHeaderPath);
        physicalHeaderFile << physicalHeader;
        physicalHeaderFile.close();
        
        // Generate physical function implementation
        std::string physicalImplTemplate = readTemplate("templates/MEOSPhysicalFunction.cpp.template");
        if (physicalImplTemplate.empty()) return false;
        
        std::string physicalImpl = replaceTemplatePlaceholders(physicalImplTemplate, func);
        std::string physicalImplPath = physicalSrcDir + "/" + func.physical_class + ".cpp";
        
        std::ofstream physicalImplFile(physicalImplPath);
        physicalImplFile << physicalImpl;
        physicalImplFile.close();
        
        std::cout << "Generated function " << func.name << " in " << outputDir << std::endl;
        std::cout << "  - " << logicalHeaderPath << std::endl;
        std::cout << "  - " << logicalImplPath << std::endl;
        std::cout << "  - " << physicalHeaderPath << std::endl;
        std::cout << "  - " << physicalImplPath << std::endl;
        
        return true;
    }

private:
    bool updateAntlrGrammar(const std::string& grammarPath) {
        std::cout << "DEBUG: Updating grammar file (bulk): " << grammarPath << std::endl;
        
        // Read existing grammar file
        std::ifstream file(grammarPath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << grammarPath << std::endl;
            return false;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();
        
        // Process each function individually (same as single function approach)
        for (const auto& pair : functions) {
            const FunctionInfo& func = pair.second;
            
            // Generate token definition for this function using MEOS function name (same as single)
            std::string upperToken = func.name;
            std::transform(upperToken.begin(), upperToken.end(), upperToken.begin(), ::toupper);
            std::string lowerToken = func.name;
            std::transform(lowerToken.begin(), lowerToken.end(), lowerToken.begin(), ::tolower);
            
            std::string tokenDefinition = upperToken + ": '" + upperToken + "' | '" + lowerToken + "';";
            
            // Check if token already exists (exact match with word boundaries)
            std::regex tokenRegex("\\b" + upperToken + ":");
            if (std::regex_search(content, tokenRegex)) {
                std::cout << "Token " << upperToken << " already exists in grammar" << std::endl;
                continue;
            }
            
            // Add token definition before WATERMARK token (same position as single)
            size_t insertPos = content.find("WATERMARK: 'WATERMARK' | 'watermark';");
            if (insertPos == std::string::npos) {
                std::cerr << "Error: Could not find WATERMARK token to position new token" << std::endl;
                return false;
            }
            // Insert the new token definition before WATERMARK (same as single)
            content.insert(insertPos, tokenDefinition + "\n");
            
            // Update functionName rule (same approach as single)
            std::regex functionNameRegex("(functionName:\\s*[^;]+);");
            std::smatch match;
            
            if (std::regex_search(content, match, functionNameRegex)) {
                std::string existingRule = match[1].str();
                std::string newRule = existingRule + " | " + upperToken + ";";
                content = std::regex_replace(content, functionNameRegex, newRule);
            }
        }
        
        // Write back to file
        std::ofstream outFile(grammarPath);
        outFile << content;
        outFile.close();
        
        std::cout << "Updated ANTLR grammar: " << grammarPath << std::endl;
        return true;
    }
    
    bool updateQueryPlanCreator(const std::string& queryPlanCreatorPath) {
        std::cout << "DEBUG: Updating query plan creator (bulk): " << queryPlanCreatorPath << std::endl;
        
        // Read the template
        std::string queryPlanTemplate = readTemplate("templates/QueryPlanCreator.template");
        if (queryPlanTemplate.empty()) {
            std::cerr << "Error: Cannot read QueryPlanCreator template" << std::endl;
            return false;
        }
        
        // Read existing file
        std::ifstream file(queryPlanCreatorPath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << queryPlanCreatorPath << std::endl;
            return false;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();
        
        // Process each function individually (same as single function approach)
        for (const auto& pair : functions) {
            const FunctionInfo& func = pair.second;
            
            // Generate case handler from template (same as single)
            std::string caseHandler = replaceTemplatePlaceholders(queryPlanTemplate, func);
            std::string include = "#include <Functions/Meos/" + func.logical_class + ".hpp>";
            
            // Check if function already exists (exact match)
            std::string upperToken = func.name;
            std::transform(upperToken.begin(), upperToken.end(), upperToken.begin(), ::toupper);
            std::string casePattern = "case AntlrSQLLexer::" + upperToken + ":";
            if (content.find(casePattern) != std::string::npos) {
                std::cout << "Function " << upperToken << " already exists in query plan creator" << std::endl;
                continue;
            }
            
            // Add include near other MEOS includes (same approach as single)
            std::regex includeRegex("(#include <Functions/Meos/[^>]+>)");
            std::string lastInclude;
            std::sregex_iterator iter(content.begin(), content.end(), includeRegex);
            std::sregex_iterator end;
            
            for (; iter != end; ++iter) {
                lastInclude = iter->str();
            }
            
            if (!lastInclude.empty()) {
                content = std::regex_replace(content, std::regex(regex_escape(lastInclude)), 
                                           lastInclude + "\n" + include);
            }
            
            // Find the LAST default case in the switch statement (same as single)
            std::regex defaultRegex("(\\s{8}default:\\s*\\n)");
            std::sregex_iterator defaultIter(content.begin(), content.end(), defaultRegex);
            std::sregex_iterator defaultEnd;
            
            std::string lastDefaultLine;
            size_t lastDefaultPos = 0;
            
            // Find the last occurrence of default case
            for (; defaultIter != defaultEnd; ++defaultIter) {
                size_t pos = defaultIter->position();
                if (pos > lastDefaultPos) {
                    lastDefaultPos = pos;
                    lastDefaultLine = defaultIter->str();
                }
            }
            
            if (!lastDefaultLine.empty()) {
                std::string replacement = caseHandler + "\n" + lastDefaultLine;
                // Replace only the last occurrence
                size_t pos = content.rfind(lastDefaultLine);
                if (pos != std::string::npos) {
                    content.replace(pos, lastDefaultLine.length(), replacement);
                    std::cout << "Inserted case handler for " << upperToken << " before last default case" << std::endl;
                }
            } else {
                std::cerr << "Warning: Could not find default case for insertion" << std::endl;
            }
        }
        
        // Write back to file
        std::ofstream outFile(queryPlanCreatorPath);
        outFile << content;
        outFile.close();
        
        std::cout << "Updated query plan creator: " << queryPlanCreatorPath << std::endl;
        return true;
    }
    
    std::string regex_escape(const std::string& str) {
        std::string result;
        for (char c : str) {
            if (c == '\\' || c == '^' || c == '$' || c == '.' || c == '|' || 
                c == '?' || c == '*' || c == '+' || c == '(' || c == ')' || 
                c == '[' || c == ']' || c == '{' || c == '}') {
                result += '\\';
            }
            result += c;
        }
        return result;
    }
    
    
    std::string generateSqlToken(const FunctionInfo& func) {
        // Use the MEOS function name directly as the SQL token
        std::string upperToken = func.name;
        std::transform(upperToken.begin(), upperToken.end(), upperToken.begin(), ::toupper);
        return upperToken;
    }
    
    std::string generateSqlParameterExtractions(const FunctionInfo& func) {
        std::ostringstream oss;
        // Extract parameters in reverse order (LIFO stack) - similar to existing SQL parser pattern
        for (int i = func.param_count - 1; i >= 0; --i) {
            oss << "                const auto param" << i << "Function = helpers.top().functionBuilder.back();\n";
            oss << "                helpers.top().functionBuilder.pop_back();\n";
        }
        return oss.str();
    }
    
    // Implementation of single-function SQL parser update methods
    bool updateAntlrGrammarForFunction(const FunctionInfo& func, const std::string& grammarPath) {
        std::cout << "DEBUG: Updating grammar file: " << grammarPath << std::endl;
        std::cout << "DEBUG: Function name: " << func.name << std::endl;
        
        // Read existing grammar file
        std::ifstream file(grammarPath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << grammarPath << std::endl;
            return false;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        // Generate token definition for this function using MEOS function name
        std::string upperToken = func.name;
        std::transform(upperToken.begin(), upperToken.end(), upperToken.begin(), ::toupper);
        std::string lowerToken = func.name;
        std::transform(lowerToken.begin(), lowerToken.end(), lowerToken.begin(), ::tolower);
        
        std::string tokenDefinition = upperToken + ": '" + upperToken + "' | '" + lowerToken + "';";
        
        // Check if token already exists (exact match with word boundaries)
        std::regex tokenRegex("\\b" + upperToken + ":");
        if (std::regex_search(content, tokenRegex)) {
            std::cout << "Token " << upperToken << " already exists in grammar" << std::endl;
            return true;
        }
        
        // Add token definition before WATERMARK token (with other MEOS function tokens)
        size_t insertPos = content.find("WATERMARK: 'WATERMARK' | 'watermark';");
        if (insertPos == std::string::npos) {
            std::cerr << "Error: Could not find WATERMARK token to position new token" << std::endl;
            return false;
        }
        // Insert the new token definition before WATERMARK
        content.insert(insertPos, tokenDefinition + "\n");
        
        // Update functionName rule
        std::regex functionNameRegex("(functionName:\\s*[^;]+);");
        std::smatch match;
        
        if (std::regex_search(content, match, functionNameRegex)) {
            std::string existingRule = match[1].str();
            std::string newRule = existingRule + " | " + upperToken + ";";
            content = std::regex_replace(content, functionNameRegex, newRule);
        }
        
        // Write back to file
        std::ofstream outFile(grammarPath);
        outFile << content;
        outFile.close();
        
        std::cout << "Updated ANTLR grammar with token: " << upperToken << std::endl;
        return true;
    }
    
    bool updateQueryPlanCreatorForFunction(const FunctionInfo& func, const std::string& queryPlanCreatorPath) {
        std::cout << "DEBUG: Updating query plan creator: " << queryPlanCreatorPath << std::endl;
        std::cout << "DEBUG: Function name: " << func.name << std::endl;
        
        // Read the template
        std::string queryPlanTemplate = readTemplate("templates/QueryPlanCreator.template");
        if (queryPlanTemplate.empty()) {
            std::cerr << "Error: Cannot read QueryPlanCreator template" << std::endl;
            return false;
        }
        
        // Generate case handler from template
        std::string caseHandler = replaceTemplatePlaceholders(queryPlanTemplate, func);
        std::string include = "#include <Functions/Meos/" + func.logical_class + ".hpp>";
        
        // Read existing file
        std::ifstream file(queryPlanCreatorPath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << queryPlanCreatorPath << std::endl;
            return false;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        // Check if function already exists (exact match)
        std::string upperToken = func.name;
        std::transform(upperToken.begin(), upperToken.end(), upperToken.begin(), ::toupper);
        std::string casePattern = "case AntlrSQLLexer::" + upperToken + ":";
        if (content.find(casePattern) != std::string::npos) {
            std::cout << "Function " << upperToken << " already exists in query plan creator" << std::endl;
            return true;
        }
        
        // Add include near other MEOS includes
        std::regex includeRegex("(#include <Functions/Meos/[^>]+>)");
        std::string lastInclude;
        std::sregex_iterator iter(content.begin(), content.end(), includeRegex);
        std::sregex_iterator end;
        
        for (; iter != end; ++iter) {
            lastInclude = iter->str();
        }
        
        if (!lastInclude.empty()) {
            content = std::regex_replace(content, std::regex(regex_escape(lastInclude)), 
                                       lastInclude + "\n" + include);
        }
        
        // Find the LAST default case in the switch statement (in exitFunctionCall)
        // Look for "        default:" pattern (8 spaces + default:)
        std::regex defaultRegex("(\\s{8}default:\\s*\\n)");
        std::sregex_iterator defaultIter(content.begin(), content.end(), defaultRegex);
        std::sregex_iterator defaultEnd;
        
        std::string lastDefaultLine;
        size_t lastDefaultPos = 0;
        
        // Find the last occurrence of default case
        for (; defaultIter != defaultEnd; ++defaultIter) {
            size_t pos = defaultIter->position();
            if (pos > lastDefaultPos) {
                lastDefaultPos = pos;
                lastDefaultLine = defaultIter->str();
            }
        }
        
        if (!lastDefaultLine.empty()) {
            std::string replacement = caseHandler + "\n" + lastDefaultLine;
            // Replace only the last occurrence
            size_t pos = content.rfind(lastDefaultLine);
            if (pos != std::string::npos) {
                content.replace(pos, lastDefaultLine.length(), replacement);
                std::cout << "Inserted case handler before last default case" << std::endl;
            }
        } else {
            std::cerr << "Warning: Could not find default case for insertion" << std::endl;
        }
        
        // Write back to file
        std::ofstream outFile(queryPlanCreatorPath);
        outFile << content;
        outFile.close();
        
        std::cout << "Updated query plan creator with function: " << upperToken << std::endl;
        return true;
    }
    
private:
    bool updateLogicalCMakeLists(const FunctionInfo& func, const std::string& cmakePath) {
        std::ifstream file(cmakePath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << cmakePath << std::endl;
            return false;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        // Check if function already exists
        // Generate plugin name by removing "LogicalFunction" suffix from class name
        std::string logical_plugin_name = func.logical_class;
        const std::string suffix = "LogicalFunction";
        if (logical_plugin_name.length() >= suffix.length() && 
            logical_plugin_name.compare(logical_plugin_name.length() - suffix.length(), suffix.length(), suffix) == 0) {
            logical_plugin_name = logical_plugin_name.substr(0, logical_plugin_name.length() - suffix.length());
        }
        
        if (content.find("add_plugin(" + logical_plugin_name + " LogicalFunction") != std::string::npos) {
            std::cout << "Function " << func.logical_class << " already exists in logical CMakeLists.txt" << std::endl;
            return true;
        }
        
        // Find the last add_plugin line and add our function after it
        std::regex pluginRegex("(add_plugin\\([^)]+\\)\\s*)");
        std::sregex_iterator iter(content.begin(), content.end(), pluginRegex);
        std::sregex_iterator end;
        
        std::string lastPlugin;
        size_t lastPluginPos = 0;
        
        for (; iter != end; ++iter) {
            size_t pos = iter->position();
            if (pos > lastPluginPos) {
                lastPluginPos = pos;
                lastPlugin = iter->str();
            }
        }
        
        if (!lastPlugin.empty()) {
            std::string newPlugin = "add_plugin(" + logical_plugin_name + " LogicalFunction nes-logical-operators " + func.logical_class + ".cpp)\n";
            std::string replacement = lastPlugin + "\n" + newPlugin;
            content = std::regex_replace(content, std::regex(regex_escape(lastPlugin)), replacement);
            
            // Write back to file
            std::ofstream outFile(cmakePath);
            outFile << content;
            outFile.close();
            
            std::cout << "Added " << func.logical_class << " to logical CMakeLists.txt" << std::endl;
            return true;
        }
        
        std::cerr << "Could not find suitable insertion point in logical CMakeLists.txt" << std::endl;
        return false;
    }
    
    bool updatePhysicalCMakeLists(const FunctionInfo& func, const std::string& cmakePath) {
        std::ifstream file(cmakePath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << cmakePath << std::endl;
            return false;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        // Check if function already exists
        // Generate plugin name by removing "PhysicalFunction" suffix from class name
        std::string physical_plugin_name = func.physical_class;
        const std::string suffix = "PhysicalFunction";
        if (physical_plugin_name.length() >= suffix.length() && 
            physical_plugin_name.compare(physical_plugin_name.length() - suffix.length(), suffix.length(), suffix) == 0) {
            physical_plugin_name = physical_plugin_name.substr(0, physical_plugin_name.length() - suffix.length());
        }
        
        if (content.find("add_plugin(" + physical_plugin_name + " PhysicalFunction") != std::string::npos) {
            std::cout << "Function " << func.physical_class << " already exists in physical CMakeLists.txt" << std::endl;
            return true;
        }
        
        // Find the last add_plugin line and add our function after it
        std::regex pluginRegex("(add_plugin\\([^)]+\\)\\s*)");
        std::sregex_iterator iter(content.begin(), content.end(), pluginRegex);
        std::sregex_iterator end;
        
        std::string lastPlugin;
        size_t lastPluginPos = 0;
        
        for (; iter != end; ++iter) {
            size_t pos = iter->position();
            if (pos > lastPluginPos) {
                lastPluginPos = pos;
                lastPlugin = iter->str();
            }
        }
        
        if (!lastPlugin.empty()) {
            std::string newPlugin = "add_plugin(" + physical_plugin_name + " PhysicalFunction nes-physical-operators " + func.physical_class + ".cpp)\n";
            std::string replacement = lastPlugin + "\n" + newPlugin;
            content = std::regex_replace(content, std::regex(regex_escape(lastPlugin)), replacement);
            
            // Write back to file
            std::ofstream outFile(cmakePath);
            outFile << content;
            outFile.close();
            
            std::cout << "Added " << func.physical_class + " to physical CMakeLists.txt" << std::endl;
            return true;
        }
        
        std::cerr << "Could not find suitable insertion point in physical CMakeLists.txt" << std::endl;
        return false;
    }
    
    bool updateLogicalCMakeListsForAll(const std::string& cmakePath) {
        std::cout << "DEBUG: Updating logical CMakeLists.txt (bulk): " << cmakePath << std::endl;
        
        // Process each function individually (same as single function approach)
        for (const auto& pair : functions) {
            const FunctionInfo& func = pair.second;
            if (!updateLogicalCMakeLists(func, cmakePath)) {
                std::cerr << "Warning: Failed to update logical CMakeLists.txt for " << func.name << std::endl;
            }
        }
        
        std::cout << "Updated logical CMakeLists.txt for all functions" << std::endl;
        return true;
    }
    
    bool updatePhysicalCMakeListsForAll(const std::string& cmakePath) {
        std::cout << "DEBUG: Updating physical CMakeLists.txt (bulk): " << cmakePath << std::endl;
        
        // Process each function individually (same as single function approach)
        for (const auto& pair : functions) {
            const FunctionInfo& func = pair.second;
            if (!updatePhysicalCMakeLists(func, cmakePath)) {
                std::cerr << "Warning: Failed to update physical CMakeLists.txt for " << func.name << std::endl;
            }
        }
        
        std::cout << "Updated physical CMakeLists.txt for all functions" << std::endl;
        return true;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <function_name|all> [json_path] [output_dir]\n";
        std::cout << "       " << argv[0] << " --list [json_path]\n";
        std::cout << "\nCommands:\n";
        std::cout << "  all                    Generate all MEOS functions from JSON and update SQL parser\n";
        std::cout << "  <function_name>        Generate specific MEOS function and update SQL parser (e.g., eintersects_tgeo_geo)\n";
        std::cout << "  --list                 List all available functions\n";
        std::cout << "\nExamples:\n";
        std::cout << "  " << argv[0] << " all\n";
        std::cout << "  " << argv[0] << " eintersects_tgeo_geo\n";
        std::cout << "  " << argv[0] << " --list\n";
        return 1;
    }
    
    std::string command = argv[1];
    std::string jsonPath = (argc > 2) ? argv[2] : "meos-functions.json";
    std::string outputDir = (argc > 3) ? argv[3] : ".";
    
    MEOSFunctionGenerator generator;
    
    std::cout << "Reading MEOS functions from: " << jsonPath << std::endl;
    if (!generator.parseJSONConfig(jsonPath)) {
        return 1;
    }
    
    if (command == "--list") {
        generator.listAvailableFunctions();
        return 0;
    }
    
    if (command == "all") {
        std::cout << "Output directory: " << outputDir << std::endl;
        if (!generator.generateAllFunctions(outputDir)) {
            return 1;
        }
        
        // Also update SQL parser files when generating all functions
        std::string grammarPath = "../nes-sql-parser/AntlrSQL.g4";
        std::string queryPlanCreatorPath = "../nes-sql-parser/src/AntlrSQLQueryPlanCreator.cpp";
        
        if (!generator.generateSQLParserUpdates(grammarPath, queryPlanCreatorPath)) {
            std::cerr << "Warning: Failed to update SQL parser files" << std::endl;
        }
        
        // Update CMakeLists.txt files for all functions
        if (!generator.updateCMakeListsForAllFunctions()) {
            std::cerr << "Warning: Failed to update CMakeLists.txt files" << std::endl;
        }
    } else {
        std::cout << "Output directory: " << outputDir << std::endl;
        if (!generator.generateFunction(command, outputDir)) {
            return 1;
        }
        
        // Also update SQL parser files when generating individual functions
        std::string grammarPath = "../nes-sql-parser/AntlrSQL.g4";
        std::string queryPlanCreatorPath = "../nes-sql-parser/src/AntlrSQLQueryPlanCreator.cpp";
        
        std::cout << "Updating SQL parser files for " << command << "..." << std::endl;
        if (!generator.generateSQLParserUpdatesForFunction(command, grammarPath, queryPlanCreatorPath)) {
            std::cerr << "Warning: Failed to update SQL parser files" << std::endl;
        }
    }
    
    std::cout << "MEOS function generation complete!" << std::endl;
    return 0;
}