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
include examples/smpi/energy/f77/CMakeFiles/sef.dir/depend.make

# Include the progress variables for this target.
include examples/smpi/energy/f77/CMakeFiles/sef.dir/progress.make

# Include the compile flags for this target's objects.
include examples/smpi/energy/f77/CMakeFiles/sef.dir/flags.make

examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o: examples/smpi/energy/f77/CMakeFiles/sef.dir/flags.make
examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o: examples/smpi/energy/f77/sef.f
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building Fortran object examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f77 && /home/aaa10078nj/fpga_network/SimGrid-3.21/smpi_script/bin/smpiff  $(Fortran_DEFINES) $(Fortran_FLAGS) -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f77/sef.f -o CMakeFiles/sef.dir/sef.f.o

examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o.requires:
.PHONY : examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o.requires

examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o.provides: examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o.requires
	$(MAKE) -f examples/smpi/energy/f77/CMakeFiles/sef.dir/build.make examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o.provides.build
.PHONY : examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o.provides

examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o.provides.build: examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o

# Object files for target sef
sef_OBJECTS = \
"CMakeFiles/sef.dir/sef.f.o"

# External object files for target sef
sef_EXTERNAL_OBJECTS =

examples/smpi/energy/f77/sef: examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o
examples/smpi/energy/f77/sef: examples/smpi/energy/f77/CMakeFiles/sef.dir/build.make
examples/smpi/energy/f77/sef: lib/libsimgrid.so.3.21
examples/smpi/energy/f77/sef: examples/smpi/energy/f77/CMakeFiles/sef.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking Fortran executable sef"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f77 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sef.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/smpi/energy/f77/CMakeFiles/sef.dir/build: examples/smpi/energy/f77/sef
.PHONY : examples/smpi/energy/f77/CMakeFiles/sef.dir/build

examples/smpi/energy/f77/CMakeFiles/sef.dir/requires: examples/smpi/energy/f77/CMakeFiles/sef.dir/sef.f.o.requires
.PHONY : examples/smpi/energy/f77/CMakeFiles/sef.dir/requires

examples/smpi/energy/f77/CMakeFiles/sef.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f77 && $(CMAKE_COMMAND) -P CMakeFiles/sef.dir/cmake_clean.cmake
.PHONY : examples/smpi/energy/f77/CMakeFiles/sef.dir/clean

examples/smpi/energy/f77/CMakeFiles/sef.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f77 /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f77 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f77/CMakeFiles/sef.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/smpi/energy/f77/CMakeFiles/sef.dir/depend
