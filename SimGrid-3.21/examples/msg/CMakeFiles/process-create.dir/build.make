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
include examples/msg/CMakeFiles/process-create.dir/depend.make

# Include the progress variables for this target.
include examples/msg/CMakeFiles/process-create.dir/progress.make

# Include the compile flags for this target's objects.
include examples/msg/CMakeFiles/process-create.dir/flags.make

examples/msg/CMakeFiles/process-create.dir/process-create/process-create.c.o: examples/msg/CMakeFiles/process-create.dir/flags.make
examples/msg/CMakeFiles/process-create.dir/process-create/process-create.c.o: examples/msg/process-create/process-create.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/msg/CMakeFiles/process-create.dir/process-create/process-create.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/process-create.dir/process-create/process-create.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg/process-create/process-create.c

examples/msg/CMakeFiles/process-create.dir/process-create/process-create.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/process-create.dir/process-create/process-create.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg/process-create/process-create.c > CMakeFiles/process-create.dir/process-create/process-create.c.i

examples/msg/CMakeFiles/process-create.dir/process-create/process-create.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/process-create.dir/process-create/process-create.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg/process-create/process-create.c -o CMakeFiles/process-create.dir/process-create/process-create.c.s

# Object files for target process-create
process__create_OBJECTS = \
"CMakeFiles/process-create.dir/process-create/process-create.c.o"

# External object files for target process-create
process__create_EXTERNAL_OBJECTS =

examples/msg/process-create/process-create: examples/msg/CMakeFiles/process-create.dir/process-create/process-create.c.o
examples/msg/process-create/process-create: examples/msg/CMakeFiles/process-create.dir/build.make
examples/msg/process-create/process-create: lib/libsimgrid.so.3.21
examples/msg/process-create/process-create: examples/msg/CMakeFiles/process-create.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable process-create/process-create"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/process-create.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/msg/CMakeFiles/process-create.dir/build: examples/msg/process-create/process-create

.PHONY : examples/msg/CMakeFiles/process-create.dir/build

examples/msg/CMakeFiles/process-create.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && $(CMAKE_COMMAND) -P CMakeFiles/process-create.dir/cmake_clean.cmake
.PHONY : examples/msg/CMakeFiles/process-create.dir/clean

examples/msg/CMakeFiles/process-create.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg/CMakeFiles/process-create.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/msg/CMakeFiles/process-create.dir/depend

