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
include teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/depend.make

# Include the progress variables for this target.
include teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/flags.make

teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.o: teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/flags.make
teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.o: teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.o"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.o   -c /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest.c

teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.i"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest.c > CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.i

teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.s"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest.c -o CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.s

# Object files for target parallel_log_crashtest
parallel_log_crashtest_OBJECTS = \
"CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.o"

# External object files for target parallel_log_crashtest
parallel_log_crashtest_EXTERNAL_OBJECTS =

teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest: teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/parallel_log_crashtest/parallel_log_crashtest.c.o
teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest: teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/build.make
teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest: lib/libsimgrid.so.3.21
teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest: teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable parallel_log_crashtest/parallel_log_crashtest"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parallel_log_crashtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/build: teshsuite/xbt/parallel_log_crashtest/parallel_log_crashtest

.PHONY : teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/build

teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/clean:
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && $(CMAKE_COMMAND) -P CMakeFiles/parallel_log_crashtest.dir/cmake_clean.cmake
.PHONY : teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/clean

teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/depend:
	cd /home/kien/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/xbt/CMakeFiles/parallel_log_crashtest.dir/depend

