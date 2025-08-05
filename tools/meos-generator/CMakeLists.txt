# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#    https://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# MEOS Function Generator - Automated code generation for MEOS integration

cmake_minimum_required(VERSION 3.20)
project(meos-generator CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required dependencies
find_package(PkgConfig REQUIRED)

# Check if meos variables are available from parent scope
if(NOT DEFINED meos OR NOT DEFINED meos_INCLUDE_DIR)
    # If not found in parent scope, find it here
    find_package(meos REQUIRED)
endif()

if(NOT meos OR NOT meos_INCLUDE_DIR)
    message(FATAL_ERROR "MEOS library not found. Required for header parsing.")
endif()

# Create the generator executable
add_executable(meos-generator
    src/main.cpp
    src/MeosFunctionDiscovery.cpp
    src/CodeGenerator.cpp
    src/TemplateEngine.cpp
    src/Utils.cpp
)

# Set include directories
target_include_directories(meos-generator
    PRIVATE include
    PRIVATE ${meos_INCLUDE_DIR}
)

# Link with MEOS library for header parsing
target_link_libraries(meos-generator PRIVATE ${meos})

# Add compiler definitions
target_compile_definitions(meos-generator PRIVATE
    MEOS_INCLUDE_DIR="${meos_INCLUDE_DIR}"
)

# Install generator binary
install(TARGETS meos-generator
    RUNTIME DESTINATION bin
    COMPONENT Generator
)

# Install templates
install(DIRECTORY templates/
    DESTINATION share/meos-generator/templates
    COMPONENT Templates
)

# Option to build as standalone
option(MEOS_GENERATOR_STANDALONE "Build as standalone generator" OFF)

if(MEOS_GENERATOR_STANDALONE)
    # Add testing support
    enable_testing()
    add_subdirectory(tests)
endif()