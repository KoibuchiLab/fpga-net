# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/kien/project/fpga_network/SimGrid-3.21

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kien/project/fpga_network/SimGrid-3.21

# Include any dependencies generated for this target.
include examples/msg/CMakeFiles/trace-link-user-variables.dir/depend.make

# Include the progress variables for this target.
include examples/msg/CMakeFiles/trace-link-user-variables.dir/progress.make

# Include the compile flags for this target's objects.
include examples/msg/CMakeFiles/trace-link-user-variables.dir/flags.make

examples/msg/CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.o: examples/msg/CMakeFiles/trace-link-user-variables.dir/flags.make
examples/msg/CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.o: examples/msg/trace-link-user-variables/trace-link-user-variables.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/msg/CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.o"
	cd /home/kien/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.o   -c /home/kien/project/fpga_network/SimGrid-3.21/examples/msg/trace-link-user-variables/trace-link-user-variables.c

examples/msg/CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.i"
	cd /home/kien/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kien/project/fpga_network/SimGrid-3.21/examples/msg/trace-link-user-variables/trace-link-user-variables.c > CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.i

examples/msg/CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.s"
	cd /home/kien/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kien/project/fpga_network/SimGrid-3.21/examples/msg/trace-link-user-variables/trace-link-user-variables.c -o CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.s

# Object files for target trace-link-user-variables
trace__link__user__variables_OBJECTS = \
"CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.o"

# External object files for target trace-link-user-variables
trace__link__user__variables_EXTERNAL_OBJECTS =

examples/msg/trace-link-user-variables/trace-link-user-variables: examples/msg/CMakeFiles/trace-link-user-variables.dir/trace-link-user-variables/trace-link-user-variables.c.o
examples/msg/trace-link-user-variables/trace-link-user-variables: examples/msg/CMakeFiles/trace-link-user-variables.dir/build.make
examples/msg/trace-link-user-variables/trace-link-user-variables: lib/libsimgrid.so.3.21
examples/msg/trace-link-user-variables/trace-link-user-variables: examples/msg/CMakeFiles/trace-link-user-variables.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable trace-link-user-variables/trace-link-user-variables"
	cd /home/kien/project/fpga_network/SimGrid-3.21/examples/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/trace-link-user-variables.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/msg/CMakeFiles/trace-link-user-variables.dir/build: examples/msg/trace-link-user-variables/trace-link-user-variables

.PHONY : examples/msg/CMakeFiles/trace-link-user-variables.dir/build

examples/msg/CMakeFiles/trace-link-user-variables.dir/clean:
	cd /home/kien/project/fpga_network/SimGrid-3.21/examples/msg && $(CMAKE_COMMAND) -P CMakeFiles/trace-link-user-variables.dir/cmake_clean.cmake
.PHONY : examples/msg/CMakeFiles/trace-link-user-variables.dir/clean

examples/msg/CMakeFiles/trace-link-user-variables.dir/depend:
	cd /home/kien/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/examples/msg /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/examples/msg /home/kien/project/fpga_network/SimGrid-3.21/examples/msg/CMakeFiles/trace-link-user-variables.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/msg/CMakeFiles/trace-link-user-variables.dir/depend

