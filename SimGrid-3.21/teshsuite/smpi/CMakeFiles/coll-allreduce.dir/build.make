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
include teshsuite/smpi/CMakeFiles/coll-allreduce.dir/depend.make

# Include the progress variables for this target.
include teshsuite/smpi/CMakeFiles/coll-allreduce.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/smpi/CMakeFiles/coll-allreduce.dir/flags.make

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o: teshsuite/smpi/CMakeFiles/coll-allreduce.dir/flags.make
teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o: teshsuite/smpi/coll-allreduce/coll-allreduce.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/aaa10078nj/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi/coll-allreduce/coll-allreduce.c

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/aaa10078nj/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi/coll-allreduce/coll-allreduce.c > CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.i

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/aaa10078nj/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi/coll-allreduce/coll-allreduce.c -o CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.s

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o.requires:
.PHONY : teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o.requires

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o.provides: teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o.requires
	$(MAKE) -f teshsuite/smpi/CMakeFiles/coll-allreduce.dir/build.make teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o.provides.build
.PHONY : teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o.provides

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o.provides.build: teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o

# Object files for target coll-allreduce
coll__allreduce_OBJECTS = \
"CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o"

# External object files for target coll-allreduce
coll__allreduce_EXTERNAL_OBJECTS =

teshsuite/smpi/coll-allreduce/coll-allreduce: teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o
teshsuite/smpi/coll-allreduce/coll-allreduce: teshsuite/smpi/CMakeFiles/coll-allreduce.dir/build.make
teshsuite/smpi/coll-allreduce/coll-allreduce: lib/libsimgrid.so.3.21
teshsuite/smpi/coll-allreduce/coll-allreduce: teshsuite/smpi/CMakeFiles/coll-allreduce.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable coll-allreduce/coll-allreduce"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/coll-allreduce.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/smpi/CMakeFiles/coll-allreduce.dir/build: teshsuite/smpi/coll-allreduce/coll-allreduce
.PHONY : teshsuite/smpi/CMakeFiles/coll-allreduce.dir/build

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/requires: teshsuite/smpi/CMakeFiles/coll-allreduce.dir/coll-allreduce/coll-allreduce.c.o.requires
.PHONY : teshsuite/smpi/CMakeFiles/coll-allreduce.dir/requires

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && $(CMAKE_COMMAND) -P CMakeFiles/coll-allreduce.dir/cmake_clean.cmake
.PHONY : teshsuite/smpi/CMakeFiles/coll-allreduce.dir/clean

teshsuite/smpi/CMakeFiles/coll-allreduce.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi/CMakeFiles/coll-allreduce.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/smpi/CMakeFiles/coll-allreduce.dir/depend
