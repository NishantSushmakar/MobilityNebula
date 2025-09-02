cmake_minimum_required(VERSION 3.20)
project(MeosFunctionGenerator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(generate-meos-function-smart generate-meos-function-smart.cpp)
add_executable(parse-meos-functions parse-meos-functions.cpp)
add_executable(generate-meos-function-from-json generate-meos-function-from-json.cpp)

# Install the executables
install(TARGETS generate-meos-function-smart parse-meos-functions generate-meos-function-from-json DESTINATION bin)