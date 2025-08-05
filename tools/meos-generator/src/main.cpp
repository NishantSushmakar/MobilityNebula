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
#include "CodeGenerator.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace MeosGenerator;

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --meos-include PATH       Path to MEOS include directory\n";
    std::cout << "  --output PATH            Output directory for generated code\n";
    std::cout << "  --template PATH          Template directory path\n";
    std::cout << "  --mobility-root PATH     MobilityNebula root directory\n";
    std::cout << "  --pattern PATTERN        Function name pattern to match (can be repeated)\n";
    std::cout << "  --category CATEGORY      Function category to generate (can be repeated)\n";
    std::cout << "  --skip-existing          Skip generation if file already exists\n";
    std::cout << "  --no-tests              Don't generate test files\n";
    std::cout << "  --dry-run               Don't write files, just show what would be generated\n";
    std::cout << "  --help                  Show this help message\n";
    
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " --meos-include /usr/local/include/meos --output ./generated\n";
    std::cout << "  " << programName << " --pattern \"*intersects*\" --category spatial\n";
    std::cout << "  " << programName << " --dry-run --pattern \"*contains*\"\n";
}

int main(int argc, char* argv[]) {
    GeneratorConfig config;
    std::filesystem::path meosIncludePath;
    
    // Default paths (can be overridden by command line)
    config.templatePath = std::filesystem::current_path() / "templates";
    config.outputPath = std::filesystem::current_path() / "generated";
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--meos-include" && i + 1 < argc) {
            meosIncludePath = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            config.outputPath = argv[++i];
        } else if (arg == "--template" && i + 1 < argc) {
            config.templatePath = argv[++i];
        } else if (arg == "--mobility-root" && i + 1 < argc) {
            config.mobilityNebulaRoot = argv[++i];
        } else if (arg == "--pattern" && i + 1 < argc) {
            config.functionPatterns.push_back(argv[++i]);
        } else if (arg == "--category" && i + 1 < argc) {
            config.categories.push_back(argv[++i]);
        } else if (arg == "--skip-existing") {
            config.skipExisting = true;
        } else if (arg == "--no-tests") {
            config.generateTests = false;
        } else if (arg == "--dry-run") {
            config.dryRun = true;
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Validate required arguments
    if (meosIncludePath.empty()) {
        // Try to use compile-time defined path
        #ifdef MEOS_INCLUDE_DIR
        meosIncludePath = MEOS_INCLUDE_DIR;
        #endif
        
        if (meosIncludePath.empty()) {
            std::cerr << "Error: --meos-include is required\n";
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (!std::filesystem::exists(meosIncludePath)) {
        std::cerr << "Error: MEOS include path does not exist: " << meosIncludePath << "\n";
        return 1;
    }
    
    if (!std::filesystem::exists(config.templatePath)) {
        std::cerr << "Error: Template path does not exist: " << config.templatePath << "\n";
        return 1;
    }
    
    try {
        std::cout << "MEOS Function Generator\n";
        std::cout << "======================\n";
        std::cout << "MEOS Include: " << meosIncludePath << "\n";
        std::cout << "Output Path: " << config.outputPath << "\n";
        std::cout << "Template Path: " << config.templatePath << "\n";
        if (!config.mobilityNebulaRoot.empty()) {
            std::cout << "MobilityNebula Root: " << config.mobilityNebulaRoot << "\n";
        }
        std::cout << "\n";
        
        // Discover MEOS functions
        std::cout << "Discovering MEOS functions...\n";
        MeosFunctionDiscovery discovery(meosIncludePath);
        discovery.discoverFunctions();
        
        auto allFunctions = discovery.getFunctions();
        std::cout << "Found " << allFunctions.size() << " MEOS functions\n";
        
        // Filter functions based on patterns and categories
        std::vector<MeosFunction> functionsToGenerate;
        
        if (config.functionPatterns.empty() && config.categories.empty()) {
            // Generate all temporal geometry functions by default
            functionsToGenerate = discovery.getTemporalGeometryFunctions();
        } else {
            // Apply filters
            for (const auto& function : allFunctions) {
                bool match = false;
                
                // Check patterns
                if (!config.functionPatterns.empty()) {
                    for (const auto& pattern : config.functionPatterns) {
                        // Simple wildcard matching
                        if (pattern.find('*') != std::string::npos) {
                            std::string regex_pattern = pattern;
                            std::replace(regex_pattern.begin(), regex_pattern.end(), '*', '.');
                            // TODO: Implement proper regex matching
                            if (function.name.find(pattern.substr(1, pattern.length() - 2)) != std::string::npos) {
                                match = true;
                                break;
                            }
                        } else if (function.name == pattern) {
                            match = true;
                            break;
                        }
                    }
                }
                
                // Check categories
                if (!config.categories.empty()) {
                    for (const auto& category : config.categories) {
                        if (function.getCategory() == category) {
                            match = true;
                            break;
                        }
                    }
                }
                
                if (match) {
                    functionsToGenerate.push_back(function);
                }
            }
        }
        
        std::cout << "Selected " << functionsToGenerate.size() << " functions for generation\n";
        
        if (config.dryRun) {
            std::cout << "\nDry run mode - functions that would be generated:\n";
            for (const auto& func : functionsToGenerate) {
                std::cout << "  " << func.name << " (" << func.getCategory() << ")\n";
            }
            return 0;
        }
        
        // Generate code
        std::cout << "\nGenerating code...\n";
        CodeGenerator generator(config);
        generator.generateAll(functionsToGenerate);
        
        std::cout << "Code generation completed successfully!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}