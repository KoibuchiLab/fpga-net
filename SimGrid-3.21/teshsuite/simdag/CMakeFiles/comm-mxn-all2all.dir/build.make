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
include teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/depend.make

# Include the progress variables for this target.
include teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/flags.make

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o: teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/flags.make
teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o: teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all.c

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all.c > CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.i

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all.c -o CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.s

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o.requires:
.PHONY : teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o.requires

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o.provides: teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o.requires
	$(MAKE) -f teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/build.make teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o.provides.build
.PHONY : teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o.provides

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o.provides.build: teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o

# Object files for target comm-mxn-all2all
comm__mxn__all2all_OBJECTS = \
"CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o"

# External object files for target comm-mxn-all2all
comm__mxn__all2all_EXTERNAL_OBJECTS =

teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all: teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o
teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all: teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/build.make
teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all: lib/libsimgrid.so.3.21
teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all: teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable comm-mxn-all2all/comm-mxn-all2all"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/comm-mxn-all2all.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/build: teshsuite/simdag/comm-mxn-all2all/comm-mxn-all2all
.PHONY : teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/build

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/requires: teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/comm-mxn-all2all/comm-mxn-all2all.c.o.requires
.PHONY : teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/requires

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && $(CMAKE_COMMAND) -P CMakeFiles/comm-mxn-all2all.dir/cmake_clean.cmake
.PHONY : teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/clean

teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/simdag/CMakeFiles/comm-mxn-all2all.dir/depend

