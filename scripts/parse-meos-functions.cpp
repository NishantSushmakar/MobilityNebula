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
#include <regex>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>

struct MEOSFunction {
    std::string name;
    std::string return_type;
    std::vector<std::string> param_types;
    std::string full_signature;
    std::string function_family;
    int param_count;
    std::string logical_class;
    std::string physical_class;
    std::string sql_token;

    MEOSFunction() : param_count(0) {}
};

class MEOSFunctionParser {
private:
    std::vector<MEOSFunction> functions;

    std::string extractFunctionFamily(const std::string& funcName) {
        return "meos";  // All functions are MEOS functions
    }

    std::string trimWhitespace(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

    std::string toPascalCase(const std::string& func_name) {
        std::string result;
        bool capitalize_next = true;
        
        for (char c : func_name) {
            if (c == '_') {
                capitalize_next = true;
            } else {
                if (capitalize_next) {
                    result += std::toupper(c);
                    capitalize_next = false;
                } else {
                    result += c;
                }
            }
        }
        return result;
    }

    std::string cleanHeaderContent(const std::string& content) {
        std::string cleaned = content;
        
        // Remove single-line comments
        cleaned = std::regex_replace(cleaned, std::regex("//.*"), "");
        
        // Remove multi-line comments (improved pattern)
        cleaned = std::regex_replace(cleaned, std::regex("/\\*[\\s\\S]*?\\*/"), "");
        
        // Remove preprocessor directives
        cleaned = std::regex_replace(cleaned, std::regex("^[ \\t]*#.*$", std::regex::multiline), "");
        
        // Join function declarations that span multiple lines, but keep semicolons as line breaks
        // Replace newline + whitespace with space, but restore newline after semicolons
        cleaned = std::regex_replace(cleaned, std::regex("\\n[ \\t]+"), " ");
        cleaned = std::regex_replace(cleaned, std::regex(";[ ]*"), ";\n");
        
        // Normalize spaces
        cleaned = std::regex_replace(cleaned, std::regex("[ \\t]+"), " ");
        
        return cleaned;
    }

    std::vector<std::string> parseParameters(const std::string& paramStr) {
        std::vector<std::string> params;
        std::string trimmed = trimWhitespace(paramStr);
        
        if (trimmed.empty() || trimmed == "void") {
            return params;
        }

        // Split by comma and process each parameter
        std::istringstream ss(trimmed);
        std::string param;
        
        while (std::getline(ss, param, ',')) {
            param = trimWhitespace(param);
            if (!param.empty()) {
                // Extract parameter type (everything except the last word)
                std::istringstream param_ss(param);
                std::vector<std::string> parts;
                std::string word;
                
                while (param_ss >> word) {
                    parts.push_back(word);
                }
                
                if (!parts.empty()) {
                    // Last part is parameter name, everything else is type
                    std::string param_name = parts.back();
                    
                    // Handle pointers attached to parameter name
                    size_t pointer_count = 0;
                    while (pointer_count < param_name.length() && param_name[pointer_count] == '*') {
                        pointer_count++;
                    }
                    
                    std::string param_type;
                    for (size_t i = 0; i < parts.size() - 1; ++i) {
                        if (!param_type.empty()) param_type += " ";
                        param_type += parts[i];
                    }
                    
                    if (pointer_count > 0) {
                        param_type += " " + std::string(pointer_count, '*');
                    }
                    
                    if (!param_type.empty()) {
                        params.push_back(param_type);
                    }
                }
            }
        }
        
        return params;
    }

public:
    bool parseHeader(const std::string& headerPath) {
        std::ifstream file(headerPath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open " << headerPath << std::endl;
            return false;
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();

        // Clean the content
        content = cleanHeaderContent(content);
        
        // Debug: Check if geo_as_ewkb exists in cleaned content
        if (content.find("geo_as_ewkb") != std::string::npos) {
            std::cout << "DEBUG: geo_as_ewkb found in cleaned content" << std::endl;
            // Find the line containing geo_as_ewkb
            std::istringstream ss(content);
            std::string line;
            int lineNum = 0;
            while (std::getline(ss, line)) {
                lineNum++;
                if (line.find("geo_as_ewkb") != std::string::npos) {
                    std::cout << "DEBUG Line " << lineNum << ": " << line << std::endl;
                    break;
                }
            }
        } else {
            std::cout << "DEBUG: geo_as_ewkb NOT found in cleaned content" << std::endl;
        }

        // Simple regex to capture entire extern function declarations
        // Based on PyMEOS-CFFI approach: extern everything_until_semicolon;
        std::regex fullFunctionRegex(
            R"(extern\s+[^;]+;)",
            std::regex::ECMAScript
        );

        std::sregex_iterator fullIter(content.begin(), content.end(), fullFunctionRegex);
        std::sregex_iterator fullEnd;

        int matchCount = 0;
        for (; fullIter != fullEnd; ++fullIter) {
            const std::smatch& fullMatch = *fullIter;
            std::string fullDeclaration = fullMatch[0].str();
            
            // Extract function name and parameters first
            std::regex funcNameRegex(
                R"(([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)\s*;)",
                std::regex::ECMAScript
            );
            
            std::smatch funcMatch;
            if (std::regex_search(fullDeclaration, funcMatch, funcNameRegex)) {
                matchCount++;
                
                MEOSFunction func;
                func.name = trimWhitespace(funcMatch[1].str());
                std::string paramStr = funcMatch[2].str();
                
                // Extract return type by removing "extern" and everything from function name onwards
                std::string beforeFunc = fullDeclaration.substr(0, fullDeclaration.find(func.name));
                std::string raw_return_type = beforeFunc;
                raw_return_type = std::regex_replace(raw_return_type, std::regex("extern\\s+"), "");
                raw_return_type = trimWhitespace(raw_return_type);
                func.return_type = raw_return_type;
                func.param_types = parseParameters(paramStr);
                func.param_count = func.param_types.size();
                func.full_signature = fullDeclaration;
                func.function_family = extractFunctionFamily(func.name);
                
                // Generate class names and SQL token
                func.logical_class = toPascalCase(func.name) + "LogicalFunction";
                func.physical_class = toPascalCase(func.name) + "PhysicalFunction";
                
                // Convert to uppercase for SQL token
                func.sql_token = func.name;
                std::transform(func.sql_token.begin(), func.sql_token.end(), func.sql_token.begin(), ::toupper);

                // Debug: Print first few matches to see what we're getting
                if (matchCount <= 5) {
                    std::cout << "DEBUG Match " << matchCount << ": " << func.name << " -> " << func.return_type << std::endl;
                }

                // Include ALL functions from meos_geo.h
                functions.push_back(func);
            }
        }
        
        std::cout << "DEBUG: Total regex matches found: " << matchCount << std::endl;

        std::cout << "Parsed " << functions.size() << " MEOS functions" << std::endl;
        return true;
    }

    bool generateJSON(const std::string& outputPath) {
        std::ofstream file(outputPath);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot create " << outputPath << std::endl;
            return false;
        }

        file << "{\n";
        
        for (size_t i = 0; i < functions.size(); ++i) {
            const auto& func = functions[i];
            
            file << "  \"" << func.name << "\": {\n";
            file << "    \"signature\": \"" << func.full_signature << "\",\n";
            file << "    \"return_type\": \"" << func.return_type << "\",\n";
            file << "    \"param_count\": " << func.param_count << ",\n";
            file << "    \"param_types\": [";
            
            for (size_t j = 0; j < func.param_types.size(); ++j) {
                file << "\"" << func.param_types[j] << "\"";
                if (j < func.param_types.size() - 1) file << ", ";
            }
            
            file << "],\n";
            file << "    \"function_family\": \"" << func.function_family << "\",\n";
            file << "    \"logical_class\": \"" << func.logical_class << "\",\n";
            file << "    \"physical_class\": \"" << func.physical_class << "\",\n";
            file << "    \"sql_token\": \"" << func.sql_token << "\"\n";
            file << "  }";
            
            if (i < functions.size() - 1) file << ",";
            file << "\n";
        }
        
        file << "}\n";
        
        std::cout << "Generated JSON configuration with " << functions.size() << " functions" << std::endl;
        return true;
    }

    void printSummary() {
        std::map<std::string, int> familyCounts;
        std::map<std::string, int> returnTypeCounts;
        
        for (const auto& func : functions) {
            familyCounts[func.function_family]++;
            returnTypeCounts[func.return_type]++;
        }
        
        std::cout << "\n=== Function Summary ===\n";
        std::cout << "By Family:\n";
        for (const auto& pair : familyCounts) {
            std::cout << "  " << pair.first << ": " << pair.second << " functions\n";
        }
        
        std::cout << "\nBy Return Type:\n";
        for (const auto& pair : returnTypeCounts) {
            std::cout << "  " << pair.first << ": " << pair.second << " functions\n";
        }
        
        std::cout << "\nTotal functions: " << functions.size() << std::endl;
    }
};

int main(int argc, char* argv[]) {
    std::string headerPath = "/Users/nishantsushmakar/Documents/mobility_internship/MobilityDB/meos/include/meos_geo.h";
    std::string outputPath = "meos-functions.json";
    
    if (argc > 1) {
        headerPath = argv[1];
    }
    if (argc > 2) {
        outputPath = argv[2];
    }
    
    MEOSFunctionParser parser;
    
    std::cout << "Parsing MEOS header: " << headerPath << std::endl;
    if (!parser.parseHeader(headerPath)) {
        return 1;
    }
    
    parser.printSummary();
    
    std::cout << "\nGenerating JSON configuration: " << outputPath << std::endl;
    if (!parser.generateJSON(outputPath)) {
        return 1;
    }
    
    std::cout << "\nParsing complete!" << std::endl;
    return 0;
}