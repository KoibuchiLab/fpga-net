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

# Utility rule file for maintainer-clean.

# Include the progress variables for this target.
include CMakeFiles/maintainer-clean.dir/progress.make

CMakeFiles/maintainer-clean:
	/usr/bin/cmake -E remove -f src/config_unit.c
	/usr/bin/cmake -E remove -f src/cunit_unit.c
	/usr/bin/cmake -E remove -f src/dict_unit.c
	/usr/bin/cmake -E remove -f src/dynar_unit.c
	/usr/bin/cmake -E remove -f src/ex_unit.c
	/usr/bin/cmake -E remove -f src/set_unit.c
	/usr/bin/cmake -E remove -f src/simgrid_units_main.c
	/usr/bin/cmake -E remove -f src/xbt_str_unit.c
	/usr/bin/cmake -E remove -f src/xbt_synchro_unit.c
	/usr/bin/cmake -E remove -f src/mpif.f90

maintainer-clean: CMakeFiles/maintainer-clean
maintainer-clean: CMakeFiles/maintainer-clean.dir/build.make
.PHONY : maintainer-clean

# Rule to build all files generated by this target.
CMakeFiles/maintainer-clean.dir/build: maintainer-clean
.PHONY : CMakeFiles/maintainer-clean.dir/build

CMakeFiles/maintainer-clean.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/maintainer-clean.dir/cmake_clean.cmake
.PHONY : CMakeFiles/maintainer-clean.dir/clean

CMakeFiles/maintainer-clean.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles/maintainer-clean.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/maintainer-clean.dir/depend
