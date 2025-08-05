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
 * @brief Simple template engine for code generation
 * 
 * Supports variable substitution {{variable}} and simple loops {{#array}}...{{/array}}
 */
class TemplateEngine {
public:
    explicit TemplateEngine(const std::filesystem::path& templatePath);
    
    /// Render template with context data
    std::string render(const std::string& templateName, 
                      const std::unordered_map<std::string, std::string>& variables,
                      const std::unordered_map<std::string, std::vector<std::string>>& arrays = {});
    
    /// Load template from file
    std::string loadTemplate(const std::string& templateName);
    
    /// Register custom template
    void registerTemplate(const std::string& name, const std::string& content);

private:
    std::filesystem::path templatePath_;
    std::unordered_map<std::string, std::string> templateCache_;
    
    /// Substitute variables in template
    std::string substituteVariables(const std::string& template_content,
                                   const std::unordered_map<std::string, std::string>& variables);
    
    /// Process array loops in template
    std::string processArrays(const std::string& template_content,
                             const std::unordered_map<std::string, std::vector<std::string>>& arrays);
    
    /// Find and replace template markers
    std::string replaceMarkers(const std::string& content, const std::string& marker, const std::string& replacement);
    
    /// Extract array section from template
    std::pair<size_t, size_t> findArraySection(const std::string& content, const std::string& arrayName);
};

} // namespace MeosGenerator