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
include examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/depend.make

# Include the progress variables for this target.
include examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/progress.make

# Include the compile flags for this target's objects.
include examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/flags.make

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o: examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/flags.make
examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o: examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual.cpp

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual.cpp > CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.i

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual.cpp -o CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.s

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o.requires:
.PHONY : examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o.requires

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o.provides: examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o.requires
	$(MAKE) -f examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/build.make examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o.provides.build
.PHONY : examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o.provides

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o.provides.build: examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o

# Object files for target replay_multiple_manual
replay_multiple_manual_OBJECTS = \
"CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o"

# External object files for target replay_multiple_manual
replay_multiple_manual_EXTERNAL_OBJECTS =

examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual: examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o
examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual: examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/build.make
examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual: lib/libsimgrid.so.3.21
examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual: examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable replay_multiple_manual"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/replay_multiple_manual.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/build: examples/smpi/replay_multiple_manual_deploy/replay_multiple_manual
.PHONY : examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/build

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/requires: examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/replay_multiple_manual.cpp.o.requires
.PHONY : examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/requires

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy && $(CMAKE_COMMAND) -P CMakeFiles/replay_multiple_manual.dir/cmake_clean.cmake
.PHONY : examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/clean

examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/smpi/replay_multiple_manual_deploy/CMakeFiles/replay_multiple_manual.dir/depend

