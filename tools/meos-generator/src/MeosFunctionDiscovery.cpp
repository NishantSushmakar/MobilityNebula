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

#include "MeosFunctionDiscovery.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

namespace MeosGenerator {

MeosFunctionDiscovery::MeosFunctionDiscovery(const std::filesystem::path& meosIncludePath) 
    : meosIncludePath_(meosIncludePath) {
}

void MeosFunctionDiscovery::discoverFunctions() {
    functions_.clear();
    
    if (!std::filesystem::exists(meosIncludePath_)) {
        throw std::runtime_error("MEOS include path does not exist: " + meosIncludePath_.string());
    }
    
    // Recursively search for header files
    for (const auto& entry : std::filesystem::recursive_directory_iterator(meosIncludePath_)) {
        if (entry.is_regular_file() && entry.path().extension() == ".h") {
            try {
                parseHeaderFile(entry.path());
            } catch (const std::exception& e) {
                std::cerr << "Warning: Failed to parse " << entry.path() << ": " << e.what() << "\n";
            }
        }
    }
    
    std::cout << "Discovered " << functions_.size() << " MEOS functions from headers\n";
}

void MeosFunctionDiscovery::parseHeaderFile(const std::filesystem::path& headerPath) {
    std::ifstream file(headerPath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open header file: " + headerPath.string());
    }
    
    std::string line;
    int lineNumber = 0;
    bool inMultiLineComment = false;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Skip comments
        if (inMultiLineComment) {
            if (line.find("*/") != std::string::npos) {
                inMultiLineComment = false;
            }
            continue;
        }
        
        if (line.find("/*") != std::string::npos) {
            inMultiLineComment = true;
            continue;
        }
        
        if (line.find("//") != std::string::npos) {
            line = line.substr(0, line.find("//"));
        }
        
        line = Utils::trim(line);
        if (line.empty()) {
            continue;
        }
        
        // Check if this looks like a function declaration
        if (isFunctionDeclaration(line)) {
            auto function = parseFunctionDeclaration(line, headerPath.filename().string());
            if (function.has_value()) {
                functions_.push_back(function.value());
            }
        }
    }
}

bool MeosFunctionDiscovery::isFunctionDeclaration(const std::string& line) {
    // Look for patterns that indicate function declarations:
    // - Contains parentheses with parameters
    // - Ends with semicolon (for header declarations)
    // - Contains return type before function name
    // - Exclude preprocessor directives, struct/enum declarations, etc.
    
    if (line.empty() || line[0] == '#') {
        return false;
    }
    
    if (Utils::startsWith(line, "typedef") || Utils::startsWith(line, "struct") || 
        Utils::startsWith(line, "enum") || Utils::startsWith(line, "union")) {
        return false;
    }
    
    // Must have parentheses and semicolon
    if (line.find('(') == std::string::npos || line.find(')') == std::string::npos || 
        !Utils::endsWith(line, ";")) {
        return false;
    }
    
    // Should not be a function pointer typedef
    if (line.find("(*") != std::string::npos) {
        return false;
    }
    
    return true;
}

std::optional<MeosFunction> MeosFunctionDiscovery::parseFunctionDeclaration(const std::string& line, const std::string& headerFile) {
    std::string returnType, functionName, parameters;
    
    if (!extractFunctionComponents(line, returnType, functionName, parameters)) {
        return std::nullopt;
    }
    
    MeosFunction function;
    function.name = functionName;
    function.returnType = cleanType(returnType);
    function.headerFile = headerFile;
    
    // Parse parameters
    if (!parameters.empty() && parameters != "void") {
        std::vector<std::string> paramParts = Utils::split(parameters, ',');
        for (const auto& paramStr : paramParts) {
            function.parameters.push_back(parseParameter(Utils::trim(paramStr)));
        }
    }
    
    // Categorize function
    function.isGeometryFunction = Utils::contains(functionName, "geo") || Utils::contains(functionName, "spatial");
    function.isTemporalFunction = Utils::contains(functionName, "temporal") || Utils::startsWith(functionName, "t");
    
    return function;
}

bool MeosFunctionDiscovery::extractFunctionComponents(const std::string& line, std::string& returnType, 
                                                     std::string& functionName, std::string& parameters) {
    // Remove semicolon
    std::string cleanLine = line;
    if (Utils::endsWith(cleanLine, ";")) {
        cleanLine = cleanLine.substr(0, cleanLine.length() - 1);
    }
    
    // Find the parentheses
    size_t openParen = cleanLine.find('(');
    size_t closeParen = cleanLine.rfind(')');
    
    if (openParen == std::string::npos || closeParen == std::string::npos || openParen >= closeParen) {
        return false;
    }
    
    // Extract parameters
    parameters = Utils::trim(cleanLine.substr(openParen + 1, closeParen - openParen - 1));
    
    // Extract function name and return type
    std::string beforeParen = Utils::trim(cleanLine.substr(0, openParen));
    
    // Find the last space/word boundary to separate return type from function name
    size_t lastSpace = beforeParen.rfind(' ');
    size_t lastStar = beforeParen.rfind('*');
    size_t separator = std::max(lastSpace, lastStar);
    
    if (separator == std::string::npos) {
        // No return type, function name only
        functionName = beforeParen;
        returnType = "void";
    } else {
        functionName = Utils::trim(beforeParen.substr(separator + 1));
        returnType = Utils::trim(beforeParen.substr(0, separator + (lastStar == separator ? 1 : 0)));
    }
    
    return !functionName.empty();
}

MeosParameter MeosFunctionDiscovery::parseParameter(const std::string& paramStr) {
    MeosParameter param;
    std::string cleanParam = Utils::trim(paramStr);
    
    // Handle const qualifier
    if (Utils::startsWith(cleanParam, "const ")) {
        param.isConst = true;
        cleanParam = Utils::trim(cleanParam.substr(6));
    }
    
    // Find parameter name (last word)
    size_t lastSpace = cleanParam.rfind(' ');
    size_t lastStar = cleanParam.rfind('*');
    
    if (lastSpace != std::string::npos && lastSpace > lastStar) {
        param.name = Utils::trim(cleanParam.substr(lastSpace + 1));
        param.type = Utils::trim(cleanParam.substr(0, lastSpace));
    } else {
        // No name, or pointer without space
        param.name = "unnamed";
        param.type = cleanParam;
    }
    
    // Check for pointer
    if (Utils::contains(param.type, "*")) {
        param.isPointer = true;
        param.type = Utils::replaceAll(param.type, "*", "");
        param.type = Utils::trim(param.type);
    }
    
    param.type = cleanType(param.type);
    return param;
}

std::string MeosFunctionDiscovery::cleanType(const std::string& type) {
    std::string clean = Utils::trim(type);
    
    // Remove extra spaces
    std::regex multiSpace("\\s+");
    clean = std::regex_replace(clean, multiSpace, " ");
    
    return clean;
}

std::vector<MeosFunction> MeosFunctionDiscovery::getFunctionsByCategory(const std::string& category) const {
    std::vector<MeosFunction> result;
    for (const auto& func : functions_) {
        if (func.getCategory() == category) {
            result.push_back(func);
        }
    }
    return result;
}

std::vector<MeosFunction> MeosFunctionDiscovery::getFunctionsMatching(const std::string& pattern) const {
    std::vector<MeosFunction> result;
    
    // Simple wildcard matching
    std::string regexPattern = pattern;
    std::replace(regexPattern.begin(), regexPattern.end(), '*', '.');
    regexPattern = ".*" + regexPattern + ".*";
    
    try {
        std::regex regex(regexPattern, std::regex_constants::icase);
        for (const auto& func : functions_) {
            if (std::regex_match(func.name, regex)) {
                result.push_back(func);
            }
        }
    } catch (const std::exception&) {
        // Fallback to simple substring matching
        for (const auto& func : functions_) {
            if (Utils::contains(Utils::toLowerCase(func.name), Utils::toLowerCase(pattern))) {
                result.push_back(func);
            }
        }
    }
    
    return result;
}

std::vector<MeosFunction> MeosFunctionDiscovery::getTemporalGeometryFunctions() const {
    std::vector<MeosFunction> result;
    for (const auto& func : functions_) {
        if (func.isTemporalGeometryFunction()) {
            result.push_back(func);
        }
    }
    return result;
}

std::vector<MeosFunction> MeosFunctionDiscovery::getSpatialIntersectionFunctions() const {
    std::vector<MeosFunction> result;
    for (const auto& func : functions_) {
        if (func.isSpatialIntersectionFunction()) {
            result.push_back(func);
        }
    }
    return result;
}

// MeosFunction methods
std::string MeosFunction::getCategory() const {
    if (Utils::contains(name, "intersects")) {
        return "spatial_intersection";
    } else if (Utils::contains(name, "contains")) {
        return "spatial_containment";  
    } else if (Utils::contains(name, "distance")) {
        return "spatial_distance";
    } else if (Utils::startsWith(name, "tgeo_") || Utils::startsWith(name, "tpoint_")) {
        return "temporal_geometry";
    } else if (Utils::startsWith(name, "temporal_")) {
        return "temporal";
    } else if (isGeometryFunction) {
        return "geometry";
    } else {
        return "general";
    }
}

bool MeosFunction::isTemporalGeometryFunction() const {
    return Utils::startsWith(name, "tgeo_") || Utils::startsWith(name, "tpoint_") || 
           (isTemporalFunction && isGeometryFunction);
}

bool MeosFunction::isSpatialIntersectionFunction() const {
    return Utils::contains(name, "intersects") && (isGeometryFunction || isTemporalFunction);
}

std::string MeosFunction::getSignature() const {
    std::ostringstream oss;
    oss << returnType << " " << name << "(";
    
    for (size_t i = 0; i < parameters.size(); ++i) {
        const auto& param = parameters[i];
        if (param.isConst) oss << "const ";
        oss << param.type;
        if (param.isPointer) oss << "*";
        oss << " " << param.name;
        if (i < parameters.size() - 1) oss << ", ";
    }
    
    oss << ")";
    return oss.str();
}

} // namespace MeosGenerator