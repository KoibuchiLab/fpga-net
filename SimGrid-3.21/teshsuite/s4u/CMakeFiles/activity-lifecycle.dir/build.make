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
CMAKE_SOURCE_DIR = /home/kienpham/project/fpga_network/SimGrid-3.21

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kienpham/project/fpga_network/SimGrid-3.21

# Include any dependencies generated for this target.
include teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/depend.make

# Include the progress variables for this target.
include teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/flags.make

teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.o: teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/flags.make
teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.o: teshsuite/s4u/activity-lifecycle/activity-lifecycle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.o -c /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u/activity-lifecycle/activity-lifecycle.cpp

teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u/activity-lifecycle/activity-lifecycle.cpp > CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.i

teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u/activity-lifecycle/activity-lifecycle.cpp -o CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.s

# Object files for target activity-lifecycle
activity__lifecycle_OBJECTS = \
"CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.o"

# External object files for target activity-lifecycle
activity__lifecycle_EXTERNAL_OBJECTS =

teshsuite/s4u/activity-lifecycle/activity-lifecycle: teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/activity-lifecycle/activity-lifecycle.cpp.o
teshsuite/s4u/activity-lifecycle/activity-lifecycle: teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/build.make
teshsuite/s4u/activity-lifecycle/activity-lifecycle: lib/libsimgrid.so.3.21
teshsuite/s4u/activity-lifecycle/activity-lifecycle: teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable activity-lifecycle/activity-lifecycle"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/activity-lifecycle.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/build: teshsuite/s4u/activity-lifecycle/activity-lifecycle

.PHONY : teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/build

teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u && $(CMAKE_COMMAND) -P CMakeFiles/activity-lifecycle.dir/cmake_clean.cmake
.PHONY : teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/clean

teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/s4u/CMakeFiles/activity-lifecycle.dir/depend

