# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/kien/project/fpga_network/SimGrid-3.21

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kien/project/fpga_network/SimGrid-3.21

# Include any dependencies generated for this target.
include teshsuite/xbt/CMakeFiles/parmap_bench.dir/depend.make

# Include the progress variables for this target.
include teshsuite/xbt/CMakeFiles/parmap_bench.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/xbt/CMakeFiles/parmap_bench.dir/flags.make

teshsuite/xbt/CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.o: teshsuite/xbt/CMakeFiles/parmap_bench.dir/flags.make
teshsuite/xbt/CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.o: teshsuite/xbt/parmap_bench/parmap_bench.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object teshsuite/xbt/CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.o"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.o -c /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parmap_bench/parmap_bench.cpp

teshsuite/xbt/CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.i"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parmap_bench/parmap_bench.cpp > CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.i

teshsuite/xbt/CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.s"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parmap_bench/parmap_bench.cpp -o CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.s

# Object files for target parmap_bench
parmap_bench_OBJECTS = \
"CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.o"

# External object files for target parmap_bench
parmap_bench_EXTERNAL_OBJECTS =

teshsuite/xbt/parmap_bench/parmap_bench: teshsuite/xbt/CMakeFiles/parmap_bench.dir/parmap_bench/parmap_bench.cpp.o
teshsuite/xbt/parmap_bench/parmap_bench: teshsuite/xbt/CMakeFiles/parmap_bench.dir/build.make
teshsuite/xbt/parmap_bench/parmap_bench: lib/libsimgrid.so.3.21
teshsuite/xbt/parmap_bench/parmap_bench: teshsuite/xbt/CMakeFiles/parmap_bench.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable parmap_bench/parmap_bench"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parmap_bench.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/xbt/CMakeFiles/parmap_bench.dir/build: teshsuite/xbt/parmap_bench/parmap_bench

.PHONY : teshsuite/xbt/CMakeFiles/parmap_bench.dir/build

teshsuite/xbt/CMakeFiles/parmap_bench.dir/clean:
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && $(CMAKE_COMMAND) -P CMakeFiles/parmap_bench.dir/cmake_clean.cmake
.PHONY : teshsuite/xbt/CMakeFiles/parmap_bench.dir/clean

teshsuite/xbt/CMakeFiles/parmap_bench.dir/depend:
	cd /home/kien/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/CMakeFiles/parmap_bench.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/xbt/CMakeFiles/parmap_bench.dir/depend

