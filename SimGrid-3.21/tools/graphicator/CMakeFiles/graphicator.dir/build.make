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
include tools/graphicator/CMakeFiles/graphicator.dir/depend.make

# Include the progress variables for this target.
include tools/graphicator/CMakeFiles/graphicator.dir/progress.make

# Include the compile flags for this target's objects.
include tools/graphicator/CMakeFiles/graphicator.dir/flags.make

tools/graphicator/CMakeFiles/graphicator.dir/graphicator.c.o: tools/graphicator/CMakeFiles/graphicator.dir/flags.make
tools/graphicator/CMakeFiles/graphicator.dir/graphicator.c.o: tools/graphicator/graphicator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tools/graphicator/CMakeFiles/graphicator.dir/graphicator.c.o"
	cd /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/graphicator.dir/graphicator.c.o   -c /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator/graphicator.c

tools/graphicator/CMakeFiles/graphicator.dir/graphicator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/graphicator.dir/graphicator.c.i"
	cd /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator/graphicator.c > CMakeFiles/graphicator.dir/graphicator.c.i

tools/graphicator/CMakeFiles/graphicator.dir/graphicator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/graphicator.dir/graphicator.c.s"
	cd /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator/graphicator.c -o CMakeFiles/graphicator.dir/graphicator.c.s

# Object files for target graphicator
graphicator_OBJECTS = \
"CMakeFiles/graphicator.dir/graphicator.c.o"

# External object files for target graphicator
graphicator_EXTERNAL_OBJECTS =

bin/graphicator: tools/graphicator/CMakeFiles/graphicator.dir/graphicator.c.o
bin/graphicator: tools/graphicator/CMakeFiles/graphicator.dir/build.make
bin/graphicator: lib/libsimgrid.so.3.21
bin/graphicator: tools/graphicator/CMakeFiles/graphicator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../../bin/graphicator"
	cd /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/graphicator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/graphicator/CMakeFiles/graphicator.dir/build: bin/graphicator

.PHONY : tools/graphicator/CMakeFiles/graphicator.dir/build

tools/graphicator/CMakeFiles/graphicator.dir/clean:
	cd /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator && $(CMAKE_COMMAND) -P CMakeFiles/graphicator.dir/cmake_clean.cmake
.PHONY : tools/graphicator/CMakeFiles/graphicator.dir/clean

tools/graphicator/CMakeFiles/graphicator.dir/depend:
	cd /home/kien/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator /home/kien/project/fpga_network/SimGrid-3.21/tools/graphicator/CMakeFiles/graphicator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/graphicator/CMakeFiles/graphicator.dir/depend

