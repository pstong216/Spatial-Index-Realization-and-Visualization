# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/build"

# Include any dependencies generated for this target.
include CMakeFiles/hw6_app.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hw6_app.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hw6_app.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw6_app.dir/flags.make

CMakeFiles/hw6_app.dir/hw6.cpp.o: CMakeFiles/hw6_app.dir/flags.make
CMakeFiles/hw6_app.dir/hw6.cpp.o: /Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option\ B/hw6.cpp
CMakeFiles/hw6_app.dir/hw6.cpp.o: CMakeFiles/hw6_app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hw6_app.dir/hw6.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hw6_app.dir/hw6.cpp.o -MF CMakeFiles/hw6_app.dir/hw6.cpp.o.d -o CMakeFiles/hw6_app.dir/hw6.cpp.o -c "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/hw6.cpp"

CMakeFiles/hw6_app.dir/hw6.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/hw6_app.dir/hw6.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/hw6.cpp" > CMakeFiles/hw6_app.dir/hw6.cpp.i

CMakeFiles/hw6_app.dir/hw6.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/hw6_app.dir/hw6.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/hw6.cpp" -o CMakeFiles/hw6_app.dir/hw6.cpp.s

# Object files for target hw6_app
hw6_app_OBJECTS = \
"CMakeFiles/hw6_app.dir/hw6.cpp.o"

# External object files for target hw6_app
hw6_app_EXTERNAL_OBJECTS =

hw6_app: CMakeFiles/hw6_app.dir/hw6.cpp.o
hw6_app: CMakeFiles/hw6_app.dir/build.make
hw6_app: 3rd/shapeLib/libshape_lib.a
hw6_app: libhw6_lib.a
hw6_app: 3rd/freeglut/lib/libglut.a
hw6_app: /opt/homebrew/lib/libX11.dylib
hw6_app: /usr/X11R6/lib/libXrandr.dylib
hw6_app: /usr/X11R6/lib/libXxf86vm.dylib
hw6_app: /usr/X11R6/lib/libXi.dylib
hw6_app: /usr/X11R6/lib/libGL.dylib
hw6_app: CMakeFiles/hw6_app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hw6_app"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw6_app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw6_app.dir/build: hw6_app
.PHONY : CMakeFiles/hw6_app.dir/build

CMakeFiles/hw6_app.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw6_app.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw6_app.dir/clean

CMakeFiles/hw6_app.dir/depend:
	cd "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B" "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B" "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/build" "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/build" "/Users/godpst/zju/spaDatabase/hw/hw6/hw6-Option B/build/CMakeFiles/hw6_app.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/hw6_app.dir/depend

