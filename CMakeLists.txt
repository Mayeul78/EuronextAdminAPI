# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(DatabaseTest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(include /usr/local/include)

# Ajouter les fichiers source
add_executable(TestServer src/server.cpp src/DatabaseManager.cpp src/TokenManager.cpp)
target_link_libraries(TestServer pthread sqlite3)

# Ajouter Crow et Asio
target_include_directories(TestServer PRIVATE /usr/local/include/crow /usr/local/include/asio)

# Ajouter un exécutable pour les tests
add_executable(TestManager src/TestManager.cpp src/DatabaseManager.cpp)
target_link_libraries(TestManager pthread sqlite3)
