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
include examples/msg/CMakeFiles/synchro-semaphore.dir/depend.make

# Include the progress variables for this target.
include examples/msg/CMakeFiles/synchro-semaphore.dir/progress.make

# Include the compile flags for this target's objects.
include examples/msg/CMakeFiles/synchro-semaphore.dir/flags.make

examples/msg/CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.o: examples/msg/CMakeFiles/synchro-semaphore.dir/flags.make
examples/msg/CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.o: examples/msg/synchro-semaphore/synchro-semaphore.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/msg/CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg/synchro-semaphore/synchro-semaphore.c

examples/msg/CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg/synchro-semaphore/synchro-semaphore.c > CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.i

examples/msg/CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg/synchro-semaphore/synchro-semaphore.c -o CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.s

# Object files for target synchro-semaphore
synchro__semaphore_OBJECTS = \
"CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.o"

# External object files for target synchro-semaphore
synchro__semaphore_EXTERNAL_OBJECTS =

examples/msg/synchro-semaphore/synchro-semaphore: examples/msg/CMakeFiles/synchro-semaphore.dir/synchro-semaphore/synchro-semaphore.c.o
examples/msg/synchro-semaphore/synchro-semaphore: examples/msg/CMakeFiles/synchro-semaphore.dir/build.make
examples/msg/synchro-semaphore/synchro-semaphore: lib/libsimgrid.so.3.21
examples/msg/synchro-semaphore/synchro-semaphore: examples/msg/CMakeFiles/synchro-semaphore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable synchro-semaphore/synchro-semaphore"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/synchro-semaphore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/msg/CMakeFiles/synchro-semaphore.dir/build: examples/msg/synchro-semaphore/synchro-semaphore

.PHONY : examples/msg/CMakeFiles/synchro-semaphore.dir/build

examples/msg/CMakeFiles/synchro-semaphore.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg && $(CMAKE_COMMAND) -P CMakeFiles/synchro-semaphore.dir/cmake_clean.cmake
.PHONY : examples/msg/CMakeFiles/synchro-semaphore.dir/clean

examples/msg/CMakeFiles/synchro-semaphore.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg /home/kienpham/project/fpga_network/SimGrid-3.21/examples/msg/CMakeFiles/synchro-semaphore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/msg/CMakeFiles/synchro-semaphore.dir/depend

