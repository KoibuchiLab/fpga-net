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
include examples/s4u/CMakeFiles/s4u-actor-daemon.dir/depend.make

# Include the progress variables for this target.
include examples/s4u/CMakeFiles/s4u-actor-daemon.dir/progress.make

# Include the compile flags for this target's objects.
include examples/s4u/CMakeFiles/s4u-actor-daemon.dir/flags.make

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o: examples/s4u/CMakeFiles/s4u-actor-daemon.dir/flags.make
examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o: examples/s4u/actor-daemon/s4u-actor-daemon.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u/actor-daemon/s4u-actor-daemon.cpp

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u/actor-daemon/s4u-actor-daemon.cpp > CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.i

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u/actor-daemon/s4u-actor-daemon.cpp -o CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.s

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o.requires:
.PHONY : examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o.requires

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o.provides: examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o.requires
	$(MAKE) -f examples/s4u/CMakeFiles/s4u-actor-daemon.dir/build.make examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o.provides.build
.PHONY : examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o.provides

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o.provides.build: examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o

# Object files for target s4u-actor-daemon
s4u__actor__daemon_OBJECTS = \
"CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o"

# External object files for target s4u-actor-daemon
s4u__actor__daemon_EXTERNAL_OBJECTS =

examples/s4u/actor-daemon/s4u-actor-daemon: examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o
examples/s4u/actor-daemon/s4u-actor-daemon: examples/s4u/CMakeFiles/s4u-actor-daemon.dir/build.make
examples/s4u/actor-daemon/s4u-actor-daemon: lib/libsimgrid.so.3.21
examples/s4u/actor-daemon/s4u-actor-daemon: examples/s4u/CMakeFiles/s4u-actor-daemon.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable actor-daemon/s4u-actor-daemon"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/s4u-actor-daemon.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/s4u/CMakeFiles/s4u-actor-daemon.dir/build: examples/s4u/actor-daemon/s4u-actor-daemon
.PHONY : examples/s4u/CMakeFiles/s4u-actor-daemon.dir/build

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/requires: examples/s4u/CMakeFiles/s4u-actor-daemon.dir/actor-daemon/s4u-actor-daemon.cpp.o.requires
.PHONY : examples/s4u/CMakeFiles/s4u-actor-daemon.dir/requires

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u && $(CMAKE_COMMAND) -P CMakeFiles/s4u-actor-daemon.dir/cmake_clean.cmake
.PHONY : examples/s4u/CMakeFiles/s4u-actor-daemon.dir/clean

examples/s4u/CMakeFiles/s4u-actor-daemon.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/s4u/CMakeFiles/s4u-actor-daemon.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/s4u/CMakeFiles/s4u-actor-daemon.dir/depend

