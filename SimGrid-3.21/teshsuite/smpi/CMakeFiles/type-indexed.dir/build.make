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
include teshsuite/smpi/CMakeFiles/type-indexed.dir/depend.make

# Include the progress variables for this target.
include teshsuite/smpi/CMakeFiles/type-indexed.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/smpi/CMakeFiles/type-indexed.dir/flags.make

teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o: teshsuite/smpi/CMakeFiles/type-indexed.dir/flags.make
teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o: teshsuite/smpi/type-indexed/type-indexed.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/aaa10078nj/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi/type-indexed/type-indexed.c

teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/aaa10078nj/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi/type-indexed/type-indexed.c > CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.i

teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/aaa10078nj/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi/type-indexed/type-indexed.c -o CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.s

teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o.requires:
.PHONY : teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o.requires

teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o.provides: teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o.requires
	$(MAKE) -f teshsuite/smpi/CMakeFiles/type-indexed.dir/build.make teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o.provides.build
.PHONY : teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o.provides

teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o.provides.build: teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o

# Object files for target type-indexed
type__indexed_OBJECTS = \
"CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o"

# External object files for target type-indexed
type__indexed_EXTERNAL_OBJECTS =

teshsuite/smpi/type-indexed/type-indexed: teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o
teshsuite/smpi/type-indexed/type-indexed: teshsuite/smpi/CMakeFiles/type-indexed.dir/build.make
teshsuite/smpi/type-indexed/type-indexed: lib/libsimgrid.so.3.21
teshsuite/smpi/type-indexed/type-indexed: teshsuite/smpi/CMakeFiles/type-indexed.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable type-indexed/type-indexed"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/type-indexed.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/smpi/CMakeFiles/type-indexed.dir/build: teshsuite/smpi/type-indexed/type-indexed
.PHONY : teshsuite/smpi/CMakeFiles/type-indexed.dir/build

teshsuite/smpi/CMakeFiles/type-indexed.dir/requires: teshsuite/smpi/CMakeFiles/type-indexed.dir/type-indexed/type-indexed.c.o.requires
.PHONY : teshsuite/smpi/CMakeFiles/type-indexed.dir/requires

teshsuite/smpi/CMakeFiles/type-indexed.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi && $(CMAKE_COMMAND) -P CMakeFiles/type-indexed.dir/cmake_clean.cmake
.PHONY : teshsuite/smpi/CMakeFiles/type-indexed.dir/clean

teshsuite/smpi/CMakeFiles/type-indexed.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/smpi/CMakeFiles/type-indexed.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/smpi/CMakeFiles/type-indexed.dir/depend
