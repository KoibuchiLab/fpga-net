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
include examples/simdag/CMakeFiles/sd_fail.dir/depend.make

# Include the progress variables for this target.
include examples/simdag/CMakeFiles/sd_fail.dir/progress.make

# Include the compile flags for this target's objects.
include examples/simdag/CMakeFiles/sd_fail.dir/flags.make

examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o: examples/simdag/CMakeFiles/sd_fail.dir/flags.make
examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o: examples/simdag/fail/sd_fail.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/sd_fail.dir/fail/sd_fail.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag/fail/sd_fail.c

examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sd_fail.dir/fail/sd_fail.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag/fail/sd_fail.c > CMakeFiles/sd_fail.dir/fail/sd_fail.c.i

examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sd_fail.dir/fail/sd_fail.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag/fail/sd_fail.c -o CMakeFiles/sd_fail.dir/fail/sd_fail.c.s

examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o.requires:
.PHONY : examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o.requires

examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o.provides: examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o.requires
	$(MAKE) -f examples/simdag/CMakeFiles/sd_fail.dir/build.make examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o.provides.build
.PHONY : examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o.provides

examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o.provides.build: examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o

# Object files for target sd_fail
sd_fail_OBJECTS = \
"CMakeFiles/sd_fail.dir/fail/sd_fail.c.o"

# External object files for target sd_fail
sd_fail_EXTERNAL_OBJECTS =

examples/simdag/fail/sd_fail: examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o
examples/simdag/fail/sd_fail: examples/simdag/CMakeFiles/sd_fail.dir/build.make
examples/simdag/fail/sd_fail: lib/libsimgrid.so.3.21
examples/simdag/fail/sd_fail: examples/simdag/CMakeFiles/sd_fail.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable fail/sd_fail"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sd_fail.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/simdag/CMakeFiles/sd_fail.dir/build: examples/simdag/fail/sd_fail
.PHONY : examples/simdag/CMakeFiles/sd_fail.dir/build

examples/simdag/CMakeFiles/sd_fail.dir/requires: examples/simdag/CMakeFiles/sd_fail.dir/fail/sd_fail.c.o.requires
.PHONY : examples/simdag/CMakeFiles/sd_fail.dir/requires

examples/simdag/CMakeFiles/sd_fail.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag && $(CMAKE_COMMAND) -P CMakeFiles/sd_fail.dir/cmake_clean.cmake
.PHONY : examples/simdag/CMakeFiles/sd_fail.dir/clean

examples/simdag/CMakeFiles/sd_fail.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/simdag/CMakeFiles/sd_fail.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/simdag/CMakeFiles/sd_fail.dir/depend
