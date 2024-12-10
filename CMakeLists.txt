# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(TestServer)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(include)

# Add executable
add_executable(TestServer server.cpp)

# Link libraries (e.g., pthread and sqlite3)
target_link_libraries(TestServer pthread sqlite3)
