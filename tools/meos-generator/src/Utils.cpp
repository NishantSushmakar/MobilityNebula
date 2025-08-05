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

#include "Utils.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>

namespace MeosGenerator {

std::string Utils::toCamelCase(const std::string& input) {
    if (input.empty()) return input;
    
    std::string result;
    bool nextUpper = false;
    bool first = true;
    
    for (char c : input) {
        if (c == '_' || c == '-' || c == ' ') {
            nextUpper = !first;  // Don't capitalize first letter
        } else if (nextUpper) {
            result += std::toupper(c);
            nextUpper = false;
            first = false;
        } else {
            result += first ? std::tolower(c) : c;
            first = false;
        }
    }
    
    return result;
}

std::string Utils::toPascalCase(const std::string& input) {
    if (input.empty()) return input;
    
    std::string result;
    bool nextUpper = true;
    
    for (char c : input) {
        if (c == '_' || c == '-' || c == ' ') {
            nextUpper = true;
        } else if (nextUpper) {
            result += std::toupper(c);
            nextUpper = false;
        } else {
            result += c;
        }
    }
    
    return result;
}

std::string Utils::toUpperCase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string Utils::toLowerCase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::vector<std::string> Utils::split(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::stringstream stream(input);
    std::string item;
    
    while (std::getline(stream, item, delimiter)) {
        result.push_back(item);
    }
    
    return result;
}

std::string Utils::trim(const std::string& input) {
    if (input.empty()) return input;
    
    size_t start = input.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    
    size_t end = input.find_last_not_of(" \t\n\r");
    return input.substr(start, end - start + 1);
}

std::string Utils::replaceAll(const std::string& input, const std::string& from, const std::string& to) {
    if (from.empty()) return input;
    
    std::string result = input;
    size_t pos = 0;
    
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    
    return result;
}

bool Utils::startsWith(const std::string& input, const std::string& prefix) {
    return input.length() >= prefix.length() && 
           input.compare(0, prefix.length(), prefix) == 0;
}

bool Utils::endsWith(const std::string& input, const std::string& suffix) {
    return input.length() >= suffix.length() &&
           input.compare(input.length() - suffix.length(), suffix.length(), suffix) == 0;
}

bool Utils::contains(const std::string& input, const std::string& substring) {
    return input.find(substring) != std::string::npos;
}

std::string Utils::join(const std::vector<std::string>& parts, const std::string& delimiter) {
    if (parts.empty()) return "";
    
    std::ostringstream oss;
    for (size_t i = 0; i < parts.size(); ++i) {
        oss << parts[i];
        if (i < parts.size() - 1) {
            oss << delimiter;
        }
    }
    
    return oss.str();
}

void Utils::ensureDirectoryExists(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }
}

std::string Utils::readFile(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path.string());
    }
    
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

void Utils::writeFile(const std::filesystem::path& path, const std::string& content) {
    ensureDirectoryExists(path.parent_path());
    
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write to file: " + path.string());
    }
    
    file << content;
}

std::string Utils::getFileExtension(const std::filesystem::path& path) {
    return path.extension().string();
}

std::string Utils::removeExtension(std::string filename) {
    size_t lastDot = filename.find_last_of('.');
    if (lastDot != std::string::npos) {
        return filename.substr(0, lastDot);
    }
    return filename;
}

std::string Utils::generateLicenseHeader() {
    return R"(/*
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

)";
}

std::string Utils::generateIncludeGuard(const std::filesystem::path& path) {
    std::string guard = path.filename().string();
    guard = replaceAll(guard, ".", "_");
    guard = replaceAll(guard, "-", "_");
    guard = toUpperCase(guard);
    return "NES_" + guard + "_";
}

std::string Utils::escapeCppString(const std::string& input) {
    std::string result;
    for (char c : input) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            case '\r': result += "\\r"; break;
            default: result += c; break;
        }
    }
    return result;
}

std::string Utils::generateUniqueId(const std::string& prefix) {
    auto now = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    
    std::ostringstream oss;
    oss << prefix << timestamp;
    return oss.str();
}

} // namespace MeosGenerator