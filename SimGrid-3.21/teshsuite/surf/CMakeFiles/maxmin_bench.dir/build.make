# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kienpham/project/fpga_network/SimGrid-3.21

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kienpham/project/fpga_network/SimGrid-3.21

# Include any dependencies generated for this target.
include teshsuite/surf/CMakeFiles/maxmin_bench.dir/depend.make

# Include the progress variables for this target.
include teshsuite/surf/CMakeFiles/maxmin_bench.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/surf/CMakeFiles/maxmin_bench.dir/flags.make

teshsuite/surf/CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.o: teshsuite/surf/CMakeFiles/maxmin_bench.dir/flags.make
teshsuite/surf/CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.o: teshsuite/surf/maxmin_bench/maxmin_bench.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object teshsuite/surf/CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.o -c /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench/maxmin_bench.cpp

teshsuite/surf/CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench/maxmin_bench.cpp > CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.i

teshsuite/surf/CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench/maxmin_bench.cpp -o CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.s

# Object files for target maxmin_bench
maxmin_bench_OBJECTS = \
"CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.o"

# External object files for target maxmin_bench
maxmin_bench_EXTERNAL_OBJECTS =

teshsuite/surf/maxmin_bench/maxmin_bench: teshsuite/surf/CMakeFiles/maxmin_bench.dir/maxmin_bench/maxmin_bench.cpp.o
teshsuite/surf/maxmin_bench/maxmin_bench: teshsuite/surf/CMakeFiles/maxmin_bench.dir/build.make
teshsuite/surf/maxmin_bench/maxmin_bench: lib/libsimgrid.so.3.21
teshsuite/surf/maxmin_bench/maxmin_bench: teshsuite/surf/CMakeFiles/maxmin_bench.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable maxmin_bench/maxmin_bench"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/maxmin_bench.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/surf/CMakeFiles/maxmin_bench.dir/build: teshsuite/surf/maxmin_bench/maxmin_bench

.PHONY : teshsuite/surf/CMakeFiles/maxmin_bench.dir/build

teshsuite/surf/CMakeFiles/maxmin_bench.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf && $(CMAKE_COMMAND) -P CMakeFiles/maxmin_bench.dir/cmake_clean.cmake
.PHONY : teshsuite/surf/CMakeFiles/maxmin_bench.dir/clean

teshsuite/surf/CMakeFiles/maxmin_bench.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/surf/CMakeFiles/maxmin_bench.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/surf/CMakeFiles/maxmin_bench.dir/depend

