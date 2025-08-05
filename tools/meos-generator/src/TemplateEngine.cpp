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

#include "TemplateEngine.hpp"
#include "Utils.hpp"

#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace MeosGenerator {

TemplateEngine::TemplateEngine(const std::filesystem::path& templatePath) 
    : templatePath_(templatePath) {
}

std::string TemplateEngine::render(const std::string& templateName, 
                                  const std::unordered_map<std::string, std::string>& variables,
                                  const std::unordered_map<std::string, std::vector<std::string>>& arrays) {
    
    std::string templateContent = loadTemplate(templateName);
    
    // First process arrays (loops)
    templateContent = processArrays(templateContent, arrays);
    
    // Then substitute variables
    templateContent = substituteVariables(templateContent, variables);
    
    return templateContent;
}

std::string TemplateEngine::loadTemplate(const std::string& templateName) {
    // Check cache first
    auto it = templateCache_.find(templateName);
    if (it != templateCache_.end()) {
        return it->second;
    }
    
    // Load from file
    std::filesystem::path templateFile = templatePath_ / templateName;
    if (!std::filesystem::exists(templateFile)) {
        throw std::runtime_error("Template file not found: " + templateFile.string());
    }
    
    std::string content = Utils::readFile(templateFile);
    templateCache_[templateName] = content;
    return content;
}

void TemplateEngine::registerTemplate(const std::string& name, const std::string& content) {
    templateCache_[name] = content;
}

std::string TemplateEngine::substituteVariables(const std::string& template_content,
                                               const std::unordered_map<std::string, std::string>& variables) {
    std::string result = template_content;
    
    // Replace {{variable}} patterns
    std::regex variableRegex(R"(\{\{(\w+)\}\})");
    std::smatch match;
    
    std::string::const_iterator searchStart = result.cbegin();
    std::ostringstream output;
    
    while (std::regex_search(searchStart, result.cend(), match, variableRegex)) {
        // Add text before the match
        output << std::string(searchStart, match.prefix().second);
        
        // Get variable name and substitute
        std::string varName = match[1];
        auto varIt = variables.find(varName);
        if (varIt != variables.end()) {
            output << varIt->second;
        } else {
            // Keep original placeholder if variable not found
            output << match[0];
        }
        
        // Move search position
        searchStart = match.suffix().first;
    }
    
    // Add remaining text
    output << std::string(searchStart, result.cend());
    
    return output.str();
}

std::string TemplateEngine::processArrays(const std::string& template_content,
                                         const std::unordered_map<std::string, std::vector<std::string>>& arrays) {
    std::string result = template_content;
    
    // Process {{#arrayName}}...{{/arrayName}} patterns
    for (const auto& [arrayName, arrayValues] : arrays) {
        std::string startPattern = "{{#" + arrayName + "}}";
        std::string endPattern = "{{/" + arrayName + "}}";
        
        size_t startPos = result.find(startPattern);
        while (startPos != std::string::npos) {
            size_t endPos = result.find(endPattern, startPos);
            if (endPos == std::string::npos) {
                throw std::runtime_error("Missing closing tag for array: " + arrayName);
            }
            
            // Extract the template section
            size_t sectionStart = startPos + startPattern.length();
            std::string sectionTemplate = result.substr(sectionStart, endPos - sectionStart);
            
            // Repeat the section for each array element
            std::ostringstream expandedSection;
            for (size_t i = 0; i < arrayValues.size(); ++i) {
                std::string itemContent = sectionTemplate;
                
                // Replace special variables in array context
                std::unordered_map<std::string, std::string> itemVars = {
                    {"item", arrayValues[i]},
                    {"index", std::to_string(i)},
                    {"notLast", (i < arrayValues.size() - 1) ? "true" : ""}
                };
                
                itemContent = substituteVariables(itemContent, itemVars);
                expandedSection << itemContent;
            }
            
            // Replace the entire array section with expanded content
            size_t totalLength = endPos + endPattern.length() - startPos;
            result.replace(startPos, totalLength, expandedSection.str());
            
            // Look for more array sections
            startPos = result.find(startPattern, startPos + expandedSection.str().length());
        }
    }
    
    return result;
}

std::string TemplateEngine::replaceMarkers(const std::string& content, const std::string& marker, const std::string& replacement) {
    return Utils::replaceAll(content, marker, replacement);
}

std::pair<size_t, size_t> TemplateEngine::findArraySection(const std::string& content, const std::string& arrayName) {
    std::string startPattern = "{{#" + arrayName + "}}";
    std::string endPattern = "{{/" + arrayName + "}}";
    
    size_t startPos = content.find(startPattern);
    if (startPos == std::string::npos) {
        return {std::string::npos, std::string::npos};
    }
    
    size_t endPos = content.find(endPattern, startPos);
    if (endPos == std::string::npos) {
        return {std::string::npos, std::string::npos};
    }
    
    return {startPos, endPos + endPattern.length()};
}

} // namespace MeosGenerator