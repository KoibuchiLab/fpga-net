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
include teshsuite/simdag/CMakeFiles/basic5.dir/depend.make

# Include the progress variables for this target.
include teshsuite/simdag/CMakeFiles/basic5.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/simdag/CMakeFiles/basic5.dir/flags.make

teshsuite/simdag/CMakeFiles/basic5.dir/basic5/basic5.c.o: teshsuite/simdag/CMakeFiles/basic5.dir/flags.make
teshsuite/simdag/CMakeFiles/basic5.dir/basic5/basic5.c.o: teshsuite/simdag/basic5/basic5.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object teshsuite/simdag/CMakeFiles/basic5.dir/basic5/basic5.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/basic5.dir/basic5/basic5.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag/basic5/basic5.c

teshsuite/simdag/CMakeFiles/basic5.dir/basic5/basic5.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/basic5.dir/basic5/basic5.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag/basic5/basic5.c > CMakeFiles/basic5.dir/basic5/basic5.c.i

teshsuite/simdag/CMakeFiles/basic5.dir/basic5/basic5.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/basic5.dir/basic5/basic5.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag/basic5/basic5.c -o CMakeFiles/basic5.dir/basic5/basic5.c.s

# Object files for target basic5
basic5_OBJECTS = \
"CMakeFiles/basic5.dir/basic5/basic5.c.o"

# External object files for target basic5
basic5_EXTERNAL_OBJECTS =

teshsuite/simdag/basic5/basic5: teshsuite/simdag/CMakeFiles/basic5.dir/basic5/basic5.c.o
teshsuite/simdag/basic5/basic5: teshsuite/simdag/CMakeFiles/basic5.dir/build.make
teshsuite/simdag/basic5/basic5: lib/libsimgrid.so.3.21
teshsuite/simdag/basic5/basic5: teshsuite/simdag/CMakeFiles/basic5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable basic5/basic5"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/basic5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/simdag/CMakeFiles/basic5.dir/build: teshsuite/simdag/basic5/basic5

.PHONY : teshsuite/simdag/CMakeFiles/basic5.dir/build

teshsuite/simdag/CMakeFiles/basic5.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag && $(CMAKE_COMMAND) -P CMakeFiles/basic5.dir/cmake_clean.cmake
.PHONY : teshsuite/simdag/CMakeFiles/basic5.dir/clean

teshsuite/simdag/CMakeFiles/basic5.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/simdag/CMakeFiles/basic5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/simdag/CMakeFiles/basic5.dir/depend

