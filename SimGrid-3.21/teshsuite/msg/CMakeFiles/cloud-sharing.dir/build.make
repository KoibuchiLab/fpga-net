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
include teshsuite/msg/CMakeFiles/cloud-sharing.dir/depend.make

# Include the progress variables for this target.
include teshsuite/msg/CMakeFiles/cloud-sharing.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/msg/CMakeFiles/cloud-sharing.dir/flags.make

teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o: teshsuite/msg/CMakeFiles/cloud-sharing.dir/flags.make
teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o: teshsuite/msg/cloud-sharing/cloud-sharing.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/cloud-sharing/cloud-sharing.c

teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/cloud-sharing/cloud-sharing.c > CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.i

teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/cloud-sharing/cloud-sharing.c -o CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.s

teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o.requires:
.PHONY : teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o.requires

teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o.provides: teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o.requires
	$(MAKE) -f teshsuite/msg/CMakeFiles/cloud-sharing.dir/build.make teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o.provides.build
.PHONY : teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o.provides

teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o.provides.build: teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o

# Object files for target cloud-sharing
cloud__sharing_OBJECTS = \
"CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o"

# External object files for target cloud-sharing
cloud__sharing_EXTERNAL_OBJECTS =

teshsuite/msg/cloud-sharing/cloud-sharing: teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o
teshsuite/msg/cloud-sharing/cloud-sharing: teshsuite/msg/CMakeFiles/cloud-sharing.dir/build.make
teshsuite/msg/cloud-sharing/cloud-sharing: lib/libsimgrid.so.3.21
teshsuite/msg/cloud-sharing/cloud-sharing: teshsuite/msg/CMakeFiles/cloud-sharing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable cloud-sharing/cloud-sharing"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cloud-sharing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/msg/CMakeFiles/cloud-sharing.dir/build: teshsuite/msg/cloud-sharing/cloud-sharing
.PHONY : teshsuite/msg/CMakeFiles/cloud-sharing.dir/build

teshsuite/msg/CMakeFiles/cloud-sharing.dir/requires: teshsuite/msg/CMakeFiles/cloud-sharing.dir/cloud-sharing/cloud-sharing.c.o.requires
.PHONY : teshsuite/msg/CMakeFiles/cloud-sharing.dir/requires

teshsuite/msg/CMakeFiles/cloud-sharing.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -P CMakeFiles/cloud-sharing.dir/cmake_clean.cmake
.PHONY : teshsuite/msg/CMakeFiles/cloud-sharing.dir/clean

teshsuite/msg/CMakeFiles/cloud-sharing.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/msg/CMakeFiles/cloud-sharing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/msg/CMakeFiles/cloud-sharing.dir/depend
