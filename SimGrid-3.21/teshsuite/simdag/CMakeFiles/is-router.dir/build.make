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
include teshsuite/simdag/CMakeFiles/is-router.dir/depend.make

# Include the progress variables for this target.
include teshsuite/simdag/CMakeFiles/is-router.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/simdag/CMakeFiles/is-router.dir/flags.make

teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o: teshsuite/simdag/CMakeFiles/is-router.dir/flags.make
teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o: teshsuite/simdag/is-router/is-router.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/aaa10078nj/fpga_network/SimGrid-3.21/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/is-router.dir/is-router/is-router.cpp.o -c /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag/is-router/is-router.cpp

teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/is-router.dir/is-router/is-router.cpp.i"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag/is-router/is-router.cpp > CMakeFiles/is-router.dir/is-router/is-router.cpp.i

teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/is-router.dir/is-router/is-router.cpp.s"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag/is-router/is-router.cpp -o CMakeFiles/is-router.dir/is-router/is-router.cpp.s

teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o.requires:
.PHONY : teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o.requires

teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o.provides: teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o.requires
	$(MAKE) -f teshsuite/simdag/CMakeFiles/is-router.dir/build.make teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o.provides.build
.PHONY : teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o.provides

teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o.provides.build: teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o

# Object files for target is-router
is__router_OBJECTS = \
"CMakeFiles/is-router.dir/is-router/is-router.cpp.o"

# External object files for target is-router
is__router_EXTERNAL_OBJECTS =

teshsuite/simdag/is-router/is-router: teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o
teshsuite/simdag/is-router/is-router: teshsuite/simdag/CMakeFiles/is-router.dir/build.make
teshsuite/simdag/is-router/is-router: lib/libsimgrid.so.3.21
teshsuite/simdag/is-router/is-router: teshsuite/simdag/CMakeFiles/is-router.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable is-router/is-router"
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/is-router.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/simdag/CMakeFiles/is-router.dir/build: teshsuite/simdag/is-router/is-router
.PHONY : teshsuite/simdag/CMakeFiles/is-router.dir/build

teshsuite/simdag/CMakeFiles/is-router.dir/requires: teshsuite/simdag/CMakeFiles/is-router.dir/is-router/is-router.cpp.o.requires
.PHONY : teshsuite/simdag/CMakeFiles/is-router.dir/requires

teshsuite/simdag/CMakeFiles/is-router.dir/clean:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag && $(CMAKE_COMMAND) -P CMakeFiles/is-router.dir/cmake_clean.cmake
.PHONY : teshsuite/simdag/CMakeFiles/is-router.dir/clean

teshsuite/simdag/CMakeFiles/is-router.dir/depend:
	cd /home/aaa10078nj/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag /home/aaa10078nj/fpga_network/SimGrid-3.21 /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/simdag/CMakeFiles/is-router.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/simdag/CMakeFiles/is-router.dir/depend
