# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/zs/cppweb/gokesite

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/zs/cppweb/gokesite/build

# Include any dependencies generated for this target.
include CMakeFiles/gokesite.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gokesite.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gokesite.dir/flags.make

CMakeFiles/gokesite.dir/main.cpp.o: CMakeFiles/gokesite.dir/flags.make
CMakeFiles/gokesite.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/zs/cppweb/gokesite/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gokesite.dir/main.cpp.o"
	/bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gokesite.dir/main.cpp.o -c /mnt/d/zs/cppweb/gokesite/main.cpp

CMakeFiles/gokesite.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gokesite.dir/main.cpp.i"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/zs/cppweb/gokesite/main.cpp > CMakeFiles/gokesite.dir/main.cpp.i

CMakeFiles/gokesite.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gokesite.dir/main.cpp.s"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/zs/cppweb/gokesite/main.cpp -o CMakeFiles/gokesite.dir/main.cpp.s

# Object files for target gokesite
gokesite_OBJECTS = \
"CMakeFiles/gokesite.dir/main.cpp.o"

# External object files for target gokesite
gokesite_EXTERNAL_OBJECTS =

gokesite: CMakeFiles/gokesite.dir/main.cpp.o
gokesite: CMakeFiles/gokesite.dir/build.make
gokesite: CMakeFiles/gokesite.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/zs/cppweb/gokesite/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable gokesite"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gokesite.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gokesite.dir/build: gokesite

.PHONY : CMakeFiles/gokesite.dir/build

CMakeFiles/gokesite.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gokesite.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gokesite.dir/clean

CMakeFiles/gokesite.dir/depend:
	cd /mnt/d/zs/cppweb/gokesite/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/zs/cppweb/gokesite /mnt/d/zs/cppweb/gokesite /mnt/d/zs/cppweb/gokesite/build /mnt/d/zs/cppweb/gokesite/build /mnt/d/zs/cppweb/gokesite/build/CMakeFiles/gokesite.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gokesite.dir/depend
