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
include examples/msg/CMakeFiles/dht-kademlia.dir/depend.make

# Include the progress variables for this target.
include examples/msg/CMakeFiles/dht-kademlia.dir/progress.make

# Include the compile flags for this target's objects.
include examples/msg/CMakeFiles/dht-kademlia.dir/flags.make

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o: examples/msg/CMakeFiles/dht-kademlia.dir/flags.make
examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o: examples/msg/dht-kademlia/dht-kademlia.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/dht-kademlia.c

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/dht-kademlia.c > CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.i

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/dht-kademlia.c -o CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.s

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o.requires:
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o.requires

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o.provides: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o.requires
	$(MAKE) -f examples/msg/CMakeFiles/dht-kademlia.dir/build.make examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o.provides.build
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o.provides

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o.provides.build: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o: examples/msg/CMakeFiles/dht-kademlia.dir/flags.make
examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o: examples/msg/dht-kademlia/node.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/node.c

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/node.c > CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.i

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/node.c -o CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.s

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o.requires:
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o.requires

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o.provides: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o.requires
	$(MAKE) -f examples/msg/CMakeFiles/dht-kademlia.dir/build.make examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o.provides.build
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o.provides

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o.provides.build: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o: examples/msg/CMakeFiles/dht-kademlia.dir/flags.make
examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o: examples/msg/dht-kademlia/routing_table.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/routing_table.c

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/routing_table.c > CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.i

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/routing_table.c -o CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.s

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o.requires:
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o.requires

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o.provides: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o.requires
	$(MAKE) -f examples/msg/CMakeFiles/dht-kademlia.dir/build.make examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o.provides.build
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o.provides

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o.provides.build: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o: examples/msg/CMakeFiles/dht-kademlia.dir/flags.make
examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o: examples/msg/dht-kademlia/task.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/task.c

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/task.c > CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.i

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/task.c -o CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.s

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o.requires:
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o.requires

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o.provides: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o.requires
	$(MAKE) -f examples/msg/CMakeFiles/dht-kademlia.dir/build.make examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o.provides.build
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o.provides

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o.provides.build: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o: examples/msg/CMakeFiles/dht-kademlia.dir/flags.make
examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o: examples/msg/dht-kademlia/answer.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o   -c /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/answer.c

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/answer.c > CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.i

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/dht-kademlia/answer.c -o CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.s

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o.requires:
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o.requires

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o.provides: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o.requires
	$(MAKE) -f examples/msg/CMakeFiles/dht-kademlia.dir/build.make examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o.provides.build
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o.provides

examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o.provides.build: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o

# Object files for target dht-kademlia
dht__kademlia_OBJECTS = \
"CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o" \
"CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o" \
"CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o" \
"CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o" \
"CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o"

# External object files for target dht-kademlia
dht__kademlia_EXTERNAL_OBJECTS =

examples/msg/dht-kademlia/dht-kademlia: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o
examples/msg/dht-kademlia/dht-kademlia: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o
examples/msg/dht-kademlia/dht-kademlia: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o
examples/msg/dht-kademlia/dht-kademlia: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o
examples/msg/dht-kademlia/dht-kademlia: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o
examples/msg/dht-kademlia/dht-kademlia: examples/msg/CMakeFiles/dht-kademlia.dir/build.make
examples/msg/dht-kademlia/dht-kademlia: lib/libsimgrid.so.3.21
examples/msg/dht-kademlia/dht-kademlia: examples/msg/CMakeFiles/dht-kademlia.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable dht-kademlia/dht-kademlia"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dht-kademlia.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/msg/CMakeFiles/dht-kademlia.dir/build: examples/msg/dht-kademlia/dht-kademlia
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/build

examples/msg/CMakeFiles/dht-kademlia.dir/requires: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/dht-kademlia.c.o.requires
examples/msg/CMakeFiles/dht-kademlia.dir/requires: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/node.c.o.requires
examples/msg/CMakeFiles/dht-kademlia.dir/requires: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/routing_table.c.o.requires
examples/msg/CMakeFiles/dht-kademlia.dir/requires: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/task.c.o.requires
examples/msg/CMakeFiles/dht-kademlia.dir/requires: examples/msg/CMakeFiles/dht-kademlia.dir/dht-kademlia/answer.c.o.requires
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/requires

examples/msg/CMakeFiles/dht-kademlia.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg && $(CMAKE_COMMAND) -P CMakeFiles/dht-kademlia.dir/cmake_clean.cmake
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/clean

examples/msg/CMakeFiles/dht-kademlia.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/msg/CMakeFiles/dht-kademlia.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/msg/CMakeFiles/dht-kademlia.dir/depend

