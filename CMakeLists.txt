# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(DatabaseTest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(include)

add_executable(TestServer server.cpp DatabaseManager.cpp TokenManager.cpp)
target_link_libraries(TestServer pthread sqlite3)
# Add TestManager executable
add_executable(TestManager TestManager.cpp DatabaseManager.cpp)

# Link libraries for TestManager
target_link_libraries(TestManager pthread sqlite3)
