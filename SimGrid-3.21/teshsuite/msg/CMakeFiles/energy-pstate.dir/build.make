# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aaa10078nj/fpga_network/SimGrid-3.21

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aaa10078nj/fpga_network/SimGrid-3.21

# Include any dependencies generated for this target.
include teshsuite/msg/CMakeFiles/energy-pstate.dir/depend.make

# Include the progress variables for this target.
include teshsuite/msg/CMakeFiles/energy-pstate.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/msg/CMakeFiles/energy-pstate.dir/flags.make

teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o: teshsuite/msg/CMakeFiles/energy-pstate.dir/flags.make
teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o: teshsuite/msg/energy-pstate/energy-pstate.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/energy-pstate/energy-pstate.c

teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/energy-pstate/energy-pstate.c > CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.i

teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/energy-pstate/energy-pstate.c -o CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.s

teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o.requires:
.PHONY : teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o.requires

teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o.provides: teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o.requires
	$(MAKE) -f teshsuite/msg/CMakeFiles/energy-pstate.dir/build.make teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o.provides.build
.PHONY : teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o.provides

teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o.provides.build: teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o

# Object files for target energy-pstate
energy__pstate_OBJECTS = \
"CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o"

# External object files for target energy-pstate
energy__pstate_EXTERNAL_OBJECTS =

teshsuite/msg/energy-pstate/energy-pstate: teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o
teshsuite/msg/energy-pstate/energy-pstate: teshsuite/msg/CMakeFiles/energy-pstate.dir/build.make
teshsuite/msg/energy-pstate/energy-pstate: lib/libsimgrid.so.3.21
teshsuite/msg/energy-pstate/energy-pstate: teshsuite/msg/CMakeFiles/energy-pstate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable energy-pstate/energy-pstate"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/energy-pstate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/msg/CMakeFiles/energy-pstate.dir/build: teshsuite/msg/energy-pstate/energy-pstate
.PHONY : teshsuite/msg/CMakeFiles/energy-pstate.dir/build

teshsuite/msg/CMakeFiles/energy-pstate.dir/requires: teshsuite/msg/CMakeFiles/energy-pstate.dir/energy-pstate/energy-pstate.c.o.requires
.PHONY : teshsuite/msg/CMakeFiles/energy-pstate.dir/requires

teshsuite/msg/CMakeFiles/energy-pstate.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -P CMakeFiles/energy-pstate.dir/cmake_clean.cmake
.PHONY : teshsuite/msg/CMakeFiles/energy-pstate.dir/clean

teshsuite/msg/CMakeFiles/energy-pstate.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/CMakeFiles/energy-pstate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/msg/CMakeFiles/energy-pstate.dir/depend

