#pragma once

#include "MeosFunctionDiscovery.hpp"
#include "TemplateEngine.hpp"

#include <filesystem>
#include <string>
#include <unordered_map>

namespace MeosGenerator {

/**
 * @brief Configuration for code generation
 */
struct GeneratorConfig {
    std::filesystem::path outputPath;
    std::filesystem::path templatePath;
    std::filesystem::path mobilityNebulaRoot;
    
    /// Filter functions to generate (empty = all)
    std::vector<std::string> functionPatterns;
    
    /// Categories to generate (empty = all)
    std::vector<std::string> categories;
    
    /// Skip existing files
    bool skipExisting{false};
    
    /// Generate test files
    bool generateTests{true};
    
    /// Dry run mode (don't write files)
    bool dryRun{false};
};

/**
 * @brief Context data for template rendering
 */
struct TemplateContext {
    std::string functionName;
    std::string functionNameUpper;
    std::string functionNameCamelCase;
    std::string returnType;
    std::vector<MeosParameter> parameters;
    std::string meosCallName;
    std::string category;
    
    /// Template variables as key-value pairs
    std::unordered_map<std::string, std::string> variables;
    
    /// Arrays for template loops
    std::unordered_map<std::string, std::vector<std::string>> arrays;
};

/**
 * @brief Main code generator class
 */
class CodeGenerator {
public:
    explicit CodeGenerator(const GeneratorConfig& config);
    
    /// Generate code for all discovered functions
    void generateAll(const std::vector<MeosFunction>& functions);
    
    /// Generate code for a single function
    void generateFunction(const MeosFunction& function);
    
    /// Generate MEOSWrapper methods
    void generateMEOSWrapperMethods(const std::vector<MeosFunction>& functions);
    
    /// Generate logical functions
    void generateLogicalFunctions(const std::vector<MeosFunction>& functions);
    
    /// Generate physical functions
    void generatePhysicalFunctions(const std::vector<MeosFunction>& functions);
    
    /// Generate SQL grammar additions
    void generateSQLGrammar(const std::vector<MeosFunction>& functions);
    
    /// Generate SQL parser cases
    void generateSQLParserCases(const std::vector<MeosFunction>& functions);
    
    /// Generate test files
    void generateTestFiles(const std::vector<MeosFunction>& functions);
    
    /// Generate plugin registrations
    void generatePluginRegistrations(const std::vector<MeosFunction>& functions);
    
    /// Generate integration summary with instructions
    void generateIntegrationSummary(const std::vector<MeosFunction>& functions);

private:
    GeneratorConfig config_;
    TemplateEngine templateEngine_;
    
    /// Create template context from function
    TemplateContext createTemplateContext(const MeosFunction& function);
    
    /// Generate function name variations
    void generateNameVariations(TemplateContext& context, const std::string& baseName);
    
    /// Convert function name to camel case
    std::string toCamelCase(const std::string& name);
    
    /// Convert function name to upper case
    std::string toUpperCase(const std::string& name);
    
    /// Generate output file path for component
    std::filesystem::path getOutputPath(const std::string& component, const std::string& fileName);
    
    /// Check if file should be generated (respects skipExisting)
    bool shouldGenerate(const std::filesystem::path& outputPath);
    
    /// Write generated content to file
    void writeFile(const std::filesystem::path& outputPath, const std::string& content);
    
    /// Generate MEOSWrapper method for a function
    std::string generateMEOSWrapperMethod(const MeosFunction& function);
    
    /// Generate logical function class
    std::string generateLogicalFunctionClass(const MeosFunction& function);
    
    /// Generate physical function class
    std::string generatePhysicalFunctionClass(const MeosFunction& function);
    
    /// Generate SQL token for function
    std::string generateSQLToken(const MeosFunction& function);
    
    /// Generate SQL parser case
    std::string generateSQLParserCase(const MeosFunction& function);
    
    /// Generate test file content
    std::string generateTestFile(const MeosFunction& function);
    
    /// Map MEOS types to NebulaStream types
    std::string mapMeosTypeToNebulaStream(const std::string& meosType);
    
    /// Generate parameter validation code
    std::string generateParameterValidation(const std::vector<MeosParameter>& parameters);
    
    /// Generate coordinate-to-WKT conversion code
    std::string generateCoordinateConversion(const MeosFunction& function);
};

} // namespace MeosGenerator