# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.29.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.29.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build

# Include any dependencies generated for this target.
include CMakeFiles/TestServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TestServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TestServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TestServer.dir/flags.make

CMakeFiles/TestServer.dir/server.cpp.o: CMakeFiles/TestServer.dir/flags.make
CMakeFiles/TestServer.dir/server.cpp.o: /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/server.cpp
CMakeFiles/TestServer.dir/server.cpp.o: CMakeFiles/TestServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TestServer.dir/server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TestServer.dir/server.cpp.o -MF CMakeFiles/TestServer.dir/server.cpp.o.d -o CMakeFiles/TestServer.dir/server.cpp.o -c /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/server.cpp

CMakeFiles/TestServer.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TestServer.dir/server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/server.cpp > CMakeFiles/TestServer.dir/server.cpp.i

CMakeFiles/TestServer.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TestServer.dir/server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/server.cpp -o CMakeFiles/TestServer.dir/server.cpp.s

CMakeFiles/TestServer.dir/DatabaseManager.cpp.o: CMakeFiles/TestServer.dir/flags.make
CMakeFiles/TestServer.dir/DatabaseManager.cpp.o: /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/DatabaseManager.cpp
CMakeFiles/TestServer.dir/DatabaseManager.cpp.o: CMakeFiles/TestServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TestServer.dir/DatabaseManager.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TestServer.dir/DatabaseManager.cpp.o -MF CMakeFiles/TestServer.dir/DatabaseManager.cpp.o.d -o CMakeFiles/TestServer.dir/DatabaseManager.cpp.o -c /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/DatabaseManager.cpp

CMakeFiles/TestServer.dir/DatabaseManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TestServer.dir/DatabaseManager.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/DatabaseManager.cpp > CMakeFiles/TestServer.dir/DatabaseManager.cpp.i

CMakeFiles/TestServer.dir/DatabaseManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TestServer.dir/DatabaseManager.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/DatabaseManager.cpp -o CMakeFiles/TestServer.dir/DatabaseManager.cpp.s

CMakeFiles/TestServer.dir/TokenManager.cpp.o: CMakeFiles/TestServer.dir/flags.make
CMakeFiles/TestServer.dir/TokenManager.cpp.o: /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/TokenManager.cpp
CMakeFiles/TestServer.dir/TokenManager.cpp.o: CMakeFiles/TestServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TestServer.dir/TokenManager.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TestServer.dir/TokenManager.cpp.o -MF CMakeFiles/TestServer.dir/TokenManager.cpp.o.d -o CMakeFiles/TestServer.dir/TokenManager.cpp.o -c /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/TokenManager.cpp

CMakeFiles/TestServer.dir/TokenManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/TestServer.dir/TokenManager.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/TokenManager.cpp > CMakeFiles/TestServer.dir/TokenManager.cpp.i

CMakeFiles/TestServer.dir/TokenManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/TestServer.dir/TokenManager.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/TokenManager.cpp -o CMakeFiles/TestServer.dir/TokenManager.cpp.s

# Object files for target TestServer
TestServer_OBJECTS = \
"CMakeFiles/TestServer.dir/server.cpp.o" \
"CMakeFiles/TestServer.dir/DatabaseManager.cpp.o" \
"CMakeFiles/TestServer.dir/TokenManager.cpp.o"

# External object files for target TestServer
TestServer_EXTERNAL_OBJECTS =

TestServer: CMakeFiles/TestServer.dir/server.cpp.o
TestServer: CMakeFiles/TestServer.dir/DatabaseManager.cpp.o
TestServer: CMakeFiles/TestServer.dir/TokenManager.cpp.o
TestServer: CMakeFiles/TestServer.dir/build.make
TestServer: CMakeFiles/TestServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable TestServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TestServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TestServer.dir/build: TestServer
.PHONY : CMakeFiles/TestServer.dir/build

CMakeFiles/TestServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TestServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TestServer.dir/clean

CMakeFiles/TestServer.dir/depend:
	cd /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build /Users/mayeulboucher/Desktop/Project_Euronext/TestServeur/build/CMakeFiles/TestServer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/TestServer.dir/depend

