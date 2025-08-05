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

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace MeosGenerator {

/**
 * @brief Represents a MEOS function parameter
 */
struct MeosParameter {
    std::string name;
    std::string type;
    bool isPointer{false};
    bool isConst{false};
};

/**
 * @brief Represents a discovered MEOS function
 */
struct MeosFunction {
    std::string name;
    std::string returnType;
    std::vector<MeosParameter> parameters;
    std::string headerFile;
    bool isGeometryFunction{false};
    bool isTemporalFunction{false};
    
    /// Categorize function based on name patterns
    std::string getCategory() const;
    
    /// Check if function follows temporal geometry pattern (tgeo_*)
    bool isTemporalGeometryFunction() const;
    
    /// Check if function is spatial intersection function (*intersects*)
    bool isSpatialIntersectionFunction() const;
    
    /// Generate function signature string
    std::string getSignature() const;
};

/**
 * @brief Discovers MEOS functions from header files
 */
class MeosFunctionDiscovery {
public:
    explicit MeosFunctionDiscovery(const std::filesystem::path& meosIncludePath);
    
    /// Discover all MEOS functions from headers
    void discoverFunctions();
    
    /// Get all discovered functions
    const std::vector<MeosFunction>& getFunctions() const { return functions_; }
    
    /// Get functions by category
    std::vector<MeosFunction> getFunctionsByCategory(const std::string& category) const;
    
    /// Get functions matching pattern
    std::vector<MeosFunction> getFunctionsMatching(const std::string& pattern) const;
    
    /// Filter temporal geometry functions
    std::vector<MeosFunction> getTemporalGeometryFunctions() const;
    
    /// Filter spatial intersection functions
    std::vector<MeosFunction> getSpatialIntersectionFunctions() const;

private:
    std::filesystem::path meosIncludePath_;
    std::vector<MeosFunction> functions_;
    
    /// Parse a single header file for function declarations
    void parseHeaderFile(const std::filesystem::path& headerPath);
    
    /// Parse function declaration from a line
    std::optional<MeosFunction> parseFunctionDeclaration(const std::string& line, const std::string& headerFile);
    
    /// Parse parameter from parameter string
    MeosParameter parseParameter(const std::string& paramStr);
    
    /// Clean up type string (remove extra spaces, etc.)
    std::string cleanType(const std::string& type);
    
    /// Check if line contains a function declaration
    bool isFunctionDeclaration(const std::string& line);
    
    /// Extract function components from declaration line
    bool extractFunctionComponents(const std::string& line, std::string& returnType, 
                                  std::string& functionName, std::string& parameters);
};

} // namespace MeosGenerator