#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace MeosGenerator {

/**
 * @brief Utility functions for the MEOS generator
 */
class Utils {
public:
    /// Convert string to camel case (first letter lowercase)
    static std::string toCamelCase(const std::string& input);
    
    /// Convert string to Pascal case (first letter uppercase)
    static std::string toPascalCase(const std::string& input);
    
    /// Convert string to upper case
    static std::string toUpperCase(const std::string& input);
    
    /// Convert string to lower case
    static std::string toLowerCase(const std::string& input);
    
    /// Split string by delimiter
    static std::vector<std::string> split(const std::string& input, char delimiter);
    
    /// Trim whitespace from string
    static std::string trim(const std::string& input);
    
    /// Replace all occurrences of substring
    static std::string replaceAll(const std::string& input, const std::string& from, const std::string& to);
    
    /// Check if string starts with prefix
    static bool startsWith(const std::string& input, const std::string& prefix);
    
    /// Check if string ends with suffix
    static bool endsWith(const std::string& input, const std::string& suffix);
    
    /// Check if string contains substring
    static bool contains(const std::string& input, const std::string& substring);
    
    /// Join vector of strings with delimiter
    static std::string join(const std::vector<std::string>& parts, const std::string& delimiter);
    
    /// Create directory if it doesn't exist
    static void ensureDirectoryExists(const std::filesystem::path& path);
    
    /// Read entire file content
    static std::string readFile(const std::filesystem::path& path);
    
    /// Write content to file
    static void writeFile(const std::filesystem::path& path, const std::string& content);
    
    /// Get file extension
    static std::string getFileExtension(const std::filesystem::path& path);
    
    /// Remove file extension from filename
    static std::string removeExtension(std::string filename);
    
    /// Generate Apache license header
    static std::string generateLicenseHeader();
    
    /// Generate include guard name from file path
    static std::string generateIncludeGuard(const std::filesystem::path& path);
    
    /// Escape string for C++ string literal
    static std::string escapeCppString(const std::string& input);
    
    /// Generate unique identifier
    static std::string generateUniqueId(const std::string& prefix = "");
};

} // namespace MeosGenerator