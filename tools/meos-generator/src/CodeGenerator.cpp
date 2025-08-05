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

#include "CodeGenerator.hpp"
#include "Utils.hpp"

#include <iostream>
#include <sstream>

namespace MeosGenerator {

CodeGenerator::CodeGenerator(const GeneratorConfig& config) 
    : config_(config), templateEngine_(config.templatePath) {
}

void CodeGenerator::generateAll(const std::vector<MeosFunction>& functions) {
    if (functions.empty()) {
        std::cout << "No functions to generate\n";
        return;
    }
    
    std::cout << "Generating code for " << functions.size() << " functions\n";
    
    // Generate individual function components
    for (const auto& function : functions) {
        generateFunction(function);
    }
    
    // Generate collective components
    generateMEOSWrapperMethods(functions);
    generateLogicalFunctions(functions);
    generatePhysicalFunctions(functions);
    generateSQLGrammar(functions);
    generateSQLParserCases(functions);
    
    if (config_.generateTests) {
        generateTestFiles(functions);
    }
    
    // Generate plugin registrations
    generatePluginRegistrations(functions);
    
    // Generate final integration summary
    generateIntegrationSummary(functions);
    
    std::cout << "Code generation completed\n";
}

void CodeGenerator::generateFunction(const MeosFunction& function) {
    std::cout << "Generating " << function.name << " (" << function.getCategory() << ")\n";
    
    auto context = createTemplateContext(function);
    
    // Generate logical function header
    generateLogicalFunctionClass(function);
    
    // Generate physical function
    generatePhysicalFunctionClass(function);
    
    // Generate MEOSWrapper method
    generateMEOSWrapperMethod(function);
}

TemplateContext CodeGenerator::createTemplateContext(const MeosFunction& function) {
    TemplateContext context;
    
    context.functionName = function.name;
    context.returnType = function.returnType;
    context.parameters = function.parameters;
    context.meosCallName = function.name;
    context.category = function.getCategory();
    
    generateNameVariations(context, function.name);
    
    // Map MEOS types to NebulaStream types
    context.variables["returnTypeNebulaStream"] = mapMeosTypeToNebulaStream(function.returnType);
    context.variables["returnTypeCpp"] = function.returnType;
    context.variables["functionDescription"] = "Generated MEOS function: " + function.name;
    
    // Generate parameter arrays for templates
    std::vector<std::string> paramNames, paramTypes, paramDeclarations;
    for (const auto& param : function.parameters) {
        paramNames.push_back(param.name);
        paramTypes.push_back(param.type);
        
        std::string decl;
        if (param.isConst) decl += "const ";
        decl += param.type;
        if (param.isPointer) decl += "*";
        decl += " " + param.name;
        paramDeclarations.push_back(decl);
    }
    
    context.arrays["paramNames"] = paramNames;
    context.arrays["paramTypes"] = paramTypes;
    context.arrays["paramDeclarations"] = paramDeclarations;
    
    return context;
}

void CodeGenerator::generateNameVariations(TemplateContext& context, const std::string& baseName) {
    // Remove common prefixes for cleaner function names
    std::string cleanName = baseName;
    if (Utils::startsWith(cleanName, "tgeo_")) {
        cleanName = cleanName.substr(5);
    } else if (Utils::startsWith(cleanName, "tpoint_")) {
        cleanName = cleanName.substr(7);
    }
    
    context.functionNameCamelCase = Utils::toCamelCase(cleanName);
    context.functionNameUpper = Utils::toUpperCase(cleanName);
    
    context.variables["functionNameCamelCase"] = context.functionNameCamelCase;
    context.variables["functionNamePascalCase"] = Utils::toPascalCase(cleanName);
    context.variables["functionNameUpper"] = context.functionNameUpper;
    context.variables["functionName"] = baseName;
    context.variables["meosCallName"] = baseName;
}

std::filesystem::path CodeGenerator::getOutputPath(const std::string& component, const std::string& fileName) {
    std::filesystem::path basePath;
    
    if (!config_.mobilityNebulaRoot.empty()) {
        basePath = config_.mobilityNebulaRoot;
    } else {
        basePath = config_.outputPath;
    }
    
    if (component == "logical-header") {
        return basePath / "nes-logical-operators" / "include" / "Functions" / "Meos" / fileName;
    } else if (component == "logical-impl") {
        return basePath / "nes-logical-operators" / "src" / "Functions" / "Meos" / fileName;
    } else if (component == "physical-header") {
        return basePath / "nes-physical-operators" / "include" / "Functions" / "Meos" / fileName;
    } else if (component == "physical-impl") {
        return basePath / "nes-physical-operators" / "src" / "Functions" / "Meos" / fileName;
    } else if (component == "meos-wrapper") {
        return basePath / "nes-plugins" / "MEOS" / "include" / fileName;
    } else if (component == "sql-grammar") {
        return basePath / "nes-sql-parser" / fileName;
    } else if (component == "tests") {
        return basePath / "Tests" / "Functions" / fileName;
    } else {
        return config_.outputPath / component / fileName;
    }
}

bool CodeGenerator::shouldGenerate(const std::filesystem::path& outputPath) {
    if (!config_.skipExisting) {
        return true;
    }
    
    return !std::filesystem::exists(outputPath);
}

void CodeGenerator::writeFile(const std::filesystem::path& outputPath, const std::string& content) {
    if (!shouldGenerate(outputPath)) {
        std::cout << "  Skipping existing file: " << outputPath << "\n";
        return;
    }
    
    if (config_.dryRun) {
        std::cout << "  Would write: " << outputPath << "\n";
        return;
    }
    
    try {
        Utils::writeFile(outputPath, content);
        std::cout << "  Generated: " << outputPath << "\n";
    } catch (const std::exception& e) {
        std::cerr << "  Error writing " << outputPath << ": " << e.what() << "\n";
    }
}

std::string CodeGenerator::generateLogicalFunctionClass(const MeosFunction& function) {
    auto context = createTemplateContext(function);
    
    // Generate header
    std::string headerContent = templateEngine_.render("logical_function_header.hpp.template", context.variables);
    std::string headerPath = context.variables["functionNamePascalCase"] + "LogicalFunction.hpp";
    writeFile(getOutputPath("logical-header", headerPath), Utils::generateLicenseHeader() + headerContent);
    
    // Generate implementation
    std::string implContent = templateEngine_.render("logical_function_impl.cpp.template", context.variables);
    std::string implPath = context.variables["functionNamePascalCase"] + "LogicalFunction.cpp";
    writeFile(getOutputPath("logical-impl", implPath), Utils::generateLicenseHeader() + implContent);
    
    return headerContent;
}

std::string CodeGenerator::generatePhysicalFunctionClass(const MeosFunction& function) {
    auto context = createTemplateContext(function);
    
    // Generate header
    std::string headerContent = templateEngine_.render("physical_function_header.hpp.template", context.variables);
    std::string headerPath = context.variables["functionNamePascalCase"] + "PhysicalFunction.hpp";
    writeFile(getOutputPath("physical-header", headerPath), Utils::generateLicenseHeader() + headerContent);
    
    // Generate implementation
    std::string implContent = templateEngine_.render("physical_function_impl.cpp.template", context.variables);
    std::string implPath = context.variables["functionNamePascalCase"] + "PhysicalFunction.cpp";
    writeFile(getOutputPath("physical-impl", implPath), Utils::generateLicenseHeader() + implContent);
    
    return headerContent;
}

std::string CodeGenerator::generateMEOSWrapperMethod(const MeosFunction& function) {
    auto context = createTemplateContext(function);
    
    std::string methodDecl = templateEngine_.render("meos_wrapper_method.hpp.template", context.variables, context.arrays);
    std::string methodImpl = templateEngine_.render("meos_wrapper_method.cpp.template", context.variables, context.arrays);
    
    // Note: In a real implementation, we'd append these to the existing MEOSWrapper files
    // For now, we'll generate separate files
    writeFile(getOutputPath("meos-wrapper", context.variables["functionNameCamelCase"] + "_method.hpp"), methodDecl);
    writeFile(getOutputPath("meos-wrapper", context.variables["functionNameCamelCase"] + "_method.cpp"), methodImpl);
    
    return methodDecl;
}

std::string CodeGenerator::generateSQLToken(const MeosFunction& function) {
    auto context = createTemplateContext(function);
    return templateEngine_.render("sql_grammar_token.template", context.variables);
}

std::string CodeGenerator::generateSQLParserCase(const MeosFunction& function) {
    auto context = createTemplateContext(function);
    return templateEngine_.render("sql_parser_case.template", context.variables);
}

std::string CodeGenerator::generateTestFile(const MeosFunction& function) {
    auto context = createTemplateContext(function);
    
    // Add test-specific variables
    context.variables["expectedResult1"] = "1";  // Default expected results
    context.variables["expectedResult2"] = "0";
    context.variables["expectedResult3"] = "-1";
    context.variables["expectedPointResult1"] = "1";
    context.variables["expectedPointResult2"] = "0";
    context.variables["expectedPointResult3"] = "0";
    context.variables["expectedLinestringResult1"] = "0";
    context.variables["expectedLinestringResult2"] = "1";
    context.variables["expectedLinestringResult3"] = "0";
    context.variables["expectedPolygonResult1"] = "1";
    context.variables["expectedPolygonResult2"] = "1";
    context.variables["expectedPolygonResult3"] = "0";
    
    return templateEngine_.render("test_file.test.template", context.variables);
}

std::string CodeGenerator::mapMeosTypeToNebulaStream(const std::string& meosType) {
    if (meosType == "int" || meosType == "int32_t") {
        return "INT32";
    } else if (meosType == "bool") {
        return "BOOLEAN";
    } else if (meosType == "double" || meosType == "float64_t") {
        return "FLOAT64";
    } else if (meosType == "float" || meosType == "float32_t") {
        return "FLOAT32";
    } else if (meosType == "char*" || meosType == "string") {
        return "VARSIZED";
    } else {
        return "INT32";  // Default fallback
    }
}

void CodeGenerator::generateMEOSWrapperMethods(const std::vector<MeosFunction>& functions) {
    std::cout << "Generating MEOSWrapper methods for " << functions.size() << " functions\n";
    
    std::ostringstream headerMethods, implMethods;
    headerMethods << "    // Generated MEOS function methods\n";
    implMethods << "// Generated MEOS function implementations\n";
    
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        
        std::string methodDecl = templateEngine_.render("meos_wrapper_method.hpp.template", context.variables, context.arrays);
        std::string methodImpl = templateEngine_.render("meos_wrapper_method.cpp.template", context.variables, context.arrays);
        
        headerMethods << "    " << methodDecl << "\n";
        implMethods << methodImpl << "\n\n";
    }
    
    writeFile(getOutputPath("meos-wrapper", "generated_methods.hpp"), headerMethods.str());
    writeFile(getOutputPath("meos-wrapper", "generated_methods.cpp"), implMethods.str());
    
    // Generate integration instructions
    std::ostringstream instructions;
    instructions << "// Instructions for MEOSWrapper integration:\n";
    instructions << "// 1. Add generated_methods.hpp content to nes-plugins/MEOS/include/MEOSWrapper.hpp\n";
    instructions << "//    in the TemporalGeometry class declaration\n";
    instructions << "// 2. Add generated_methods.cpp content to nes-plugins/MEOS/MEOSWrapper.cpp\n";
    instructions << "//    in the implementation section\n\n";
    instructions << headerMethods.str() << "\n\n" << implMethods.str();
    
    writeFile(getOutputPath("meos-wrapper", "integration_instructions.txt"), instructions.str());
}

void CodeGenerator::generateLogicalFunctions(const std::vector<MeosFunction>& functions) {
    std::cout << "Generating logical functions for " << functions.size() << " functions\n";
    
    for (const auto& function : functions) {
        generateLogicalFunctionClass(function);
    }
}

void CodeGenerator::generatePhysicalFunctions(const std::vector<MeosFunction>& functions) {
    std::cout << "Generating physical functions for " << functions.size() << " functions\n";
    
    for (const auto& function : functions) {
        generatePhysicalFunctionClass(function);
    }
}

void CodeGenerator::generateSQLGrammar(const std::vector<MeosFunction>& functions) {
    std::cout << "Generating SQL grammar tokens for " << functions.size() << " functions\n";
    
    // Generate individual tokens for each function
    std::ostringstream tokens;
    tokens << "// Generated SQL grammar tokens for MEOS functions\n";
    
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        std::string singleToken = templateEngine_.render("sql_grammar_token.template", context.variables);
        tokens << singleToken << "\n";
    }
    
    writeFile(getOutputPath("sql-grammar", "generated_tokens.g4"), tokens.str());
    
    // Also generate instructions for manual integration
    std::ostringstream instructions;
    instructions << "// Instructions: Add these tokens to nes-sql-parser/AntlrSQL.g4\n";
    instructions << "// Add to the tokens section:\n\n";
    instructions << tokens.str() << "\n";
    instructions << "// Add to function_call rule:\n";
    instructions << "//   | TEMPORAL_*_GEOMETRY LPAREN argument (COMMA argument)* RPAREN\n";
    
    writeFile(getOutputPath("sql-grammar", "integration_instructions.txt"), instructions.str());
}

void CodeGenerator::generateSQLParserCases(const std::vector<MeosFunction>& functions) {
    std::cout << "Generating SQL parser cases for " << functions.size() << " functions\n";
    
    // Generate individual parser cases for each function
    std::ostringstream parserCases;
    parserCases << "// Generated SQL parser cases for MEOS functions\n";
    
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        std::string singleCase = templateEngine_.render("sql_parser_case.template", context.variables);
        parserCases << singleCase << "\n";
    }
    
    writeFile(getOutputPath("sql-grammar", "generated_parser_cases.cpp"), parserCases.str());
    
    // Generate includes for logical functions
    std::ostringstream includes;
    includes << "// Instructions: Add these includes to nes-sql-parser/src/AntlrSQLQueryPlanCreator.cpp\n\n";
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        includes << "#include <Functions/Meos/" << context.variables["functionNamePascalCase"] << "LogicalFunction.hpp>\n";
    }
    
    writeFile(getOutputPath("sql-grammar", "required_includes.cpp"), includes.str());
}

void CodeGenerator::generateTestFiles(const std::vector<MeosFunction>& functions) {
    std::cout << "Generating test files for " << functions.size() << " functions\n";
    
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        std::string testContent = generateTestFile(function);
        std::string testFileName = context.variables["functionNameCamelCase"] + "_geometry.test";
        writeFile(getOutputPath("tests", testFileName), testContent);
    }
}

void CodeGenerator::generatePluginRegistrations(const std::vector<MeosFunction>& functions) {
    std::cout << "Generating plugin registrations for " << functions.size() << " functions\n";
    
    // Generate logical function registrations
    std::ostringstream logicalRegistrations;
    logicalRegistrations << "// Add these to nes-logical-operators registry:\n";
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        logicalRegistrations << context.variables["functionNamePascalCase"] << "LogicalFunction,\n";
    }
    writeFile(getOutputPath("output", "logical_function_registrations.txt"), logicalRegistrations.str());
    
    // Generate physical function registrations
    std::ostringstream physicalRegistrations;
    physicalRegistrations << "// Add these to nes-physical-operators registry:\n";
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        physicalRegistrations << context.variables["functionNamePascalCase"] << "PhysicalFunction,\n";
    }
    writeFile(getOutputPath("output", "physical_function_registrations.txt"), physicalRegistrations.str());
}

void CodeGenerator::generateIntegrationSummary(const std::vector<MeosFunction>& functions) {
    std::cout << "Generating integration summary\n";
    
    std::ostringstream summary;
    summary << "# MEOS Function Integration Summary\n\n";
    summary << "Generated " << functions.size() << " MEOS functions for integration.\n\n";
    
    summary << "## Generated Components:\n";
    summary << "- " << functions.size() << " Logical Function classes (header + implementation)\n";
    summary << "- " << functions.size() << " Physical Function classes (header + implementation)\n";
    summary << "- " << functions.size() << " MEOSWrapper method declarations and implementations\n";
    summary << "- " << functions.size() << " SQL grammar tokens\n";
    summary << "- " << functions.size() << " SQL parser cases\n";
    if (config_.generateTests) {
        summary << "- " << functions.size() << " Test files\n";
    }
    summary << "\n";
    
    summary << "## Integration Steps:\n\n";
    summary << "### 1. Add Logical Functions to Build\n";
    summary << "Add generated logical function files to `nes-logical-operators/CMakeLists.txt`:\n";
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        summary << "- `src/Functions/Meos/" << context.variables["functionNamePascalCase"] << "LogicalFunction.cpp`\n";
    }
    summary << "\n";
    
    summary << "### 2. Add Physical Functions to Build\n";
    summary << "Add generated physical function files to `nes-physical-operators/CMakeLists.txt`:\n";
    for (const auto& function : functions) {
        auto context = createTemplateContext(function);
        summary << "- `src/Functions/Meos/" << context.variables["functionNamePascalCase"] << "PhysicalFunction.cpp`\n";
    }
    summary << "\n";
    
    summary << "### 3. Update MEOSWrapper\n";
    summary << "- Add content from `generated_methods.hpp` to `nes-plugins/MEOS/include/MEOSWrapper.hpp`\n";
    summary << "- Add content from `generated_methods.cpp` to `nes-plugins/MEOS/MEOSWrapper.cpp`\n\n";
    
    summary << "### 4. Update SQL Grammar\n";
    summary << "- Add tokens from `generated_tokens.g4` to `nes-sql-parser/AntlrSQL.g4`\n";
    summary << "- Add includes from `required_includes.cpp` to `nes-sql-parser/src/AntlrSQLQueryPlanCreator.cpp`\n";
    summary << "- Add parser cases from `generated_parser_cases.cpp` to the function_call switch statement\n\n";
    
    summary << "### 5. Register Functions\n";
    summary << "- Add logical function registrations to appropriate registry\n";
    summary << "- Add physical function registrations to appropriate registry\n\n";
    
    summary << "### 6. Build and Test\n";
    summary << "```bash\n";
    summary << "ninja -C build-docker\n";
    if (config_.generateTests) {
        summary << "# Run generated tests\n";
        for (const auto& function : functions) {
            auto context = createTemplateContext(function);
            summary << "ctest --test-dir build-docker -R \"" << context.variables["functionNameCamelCase"] << "\"\n";
        }
    }
    summary << "```\n\n";
    
    summary << "## Generated Functions:\n";
    for (const auto& function : functions) {
        summary << "- `" << function.name << "` (" << function.getCategory() << ")\n";
    }
    
    writeFile(getOutputPath("output", "INTEGRATION_SUMMARY.md"), summary.str());
    
    if (!config_.dryRun) {
        std::cout << "\n=== INTEGRATION SUMMARY ===\n";
        std::cout << "Generated complete integration for " << functions.size() << " MEOS functions\n";
        std::cout << "See INTEGRATION_SUMMARY.md for detailed integration steps\n";
        std::cout << "All generated files are ready for integration into MobilityNebula\n";
    }
}

} // namespace MeosGenerator