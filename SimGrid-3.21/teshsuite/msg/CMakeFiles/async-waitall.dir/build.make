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
include teshsuite/msg/CMakeFiles/async-waitall.dir/depend.make

# Include the progress variables for this target.
include teshsuite/msg/CMakeFiles/async-waitall.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/msg/CMakeFiles/async-waitall.dir/flags.make

teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o: teshsuite/msg/CMakeFiles/async-waitall.dir/flags.make
teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o: teshsuite/msg/async-waitall/async-waitall.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/async-waitall/async-waitall.c

teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/async-waitall/async-waitall.c > CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.i

teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/async-waitall/async-waitall.c -o CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.s

teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o.requires:
.PHONY : teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o.requires

teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o.provides: teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o.requires
	$(MAKE) -f teshsuite/msg/CMakeFiles/async-waitall.dir/build.make teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o.provides.build
.PHONY : teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o.provides

teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o.provides.build: teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o

# Object files for target async-waitall
async__waitall_OBJECTS = \
"CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o"

# External object files for target async-waitall
async__waitall_EXTERNAL_OBJECTS =

teshsuite/msg/async-waitall/async-waitall: teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o
teshsuite/msg/async-waitall/async-waitall: teshsuite/msg/CMakeFiles/async-waitall.dir/build.make
teshsuite/msg/async-waitall/async-waitall: lib/libsimgrid.so.3.21
teshsuite/msg/async-waitall/async-waitall: teshsuite/msg/CMakeFiles/async-waitall.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable async-waitall/async-waitall"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/async-waitall.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/msg/CMakeFiles/async-waitall.dir/build: teshsuite/msg/async-waitall/async-waitall
.PHONY : teshsuite/msg/CMakeFiles/async-waitall.dir/build

teshsuite/msg/CMakeFiles/async-waitall.dir/requires: teshsuite/msg/CMakeFiles/async-waitall.dir/async-waitall/async-waitall.c.o.requires
.PHONY : teshsuite/msg/CMakeFiles/async-waitall.dir/requires

teshsuite/msg/CMakeFiles/async-waitall.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -P CMakeFiles/async-waitall.dir/cmake_clean.cmake
.PHONY : teshsuite/msg/CMakeFiles/async-waitall.dir/clean

teshsuite/msg/CMakeFiles/async-waitall.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/CMakeFiles/async-waitall.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/msg/CMakeFiles/async-waitall.dir/depend

