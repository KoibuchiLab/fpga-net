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
include teshsuite/msg/CMakeFiles/app-pingpong.dir/depend.make

# Include the progress variables for this target.
include teshsuite/msg/CMakeFiles/app-pingpong.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/msg/CMakeFiles/app-pingpong.dir/flags.make

teshsuite/msg/CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.o: teshsuite/msg/CMakeFiles/app-pingpong.dir/flags.make
teshsuite/msg/CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.o: teshsuite/msg/app-pingpong/app-pingpong.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object teshsuite/msg/CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.o"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.o   -c /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg/app-pingpong/app-pingpong.c

teshsuite/msg/CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.i"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg/app-pingpong/app-pingpong.c > CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.i

teshsuite/msg/CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.s"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg/app-pingpong/app-pingpong.c -o CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.s

# Object files for target app-pingpong
app__pingpong_OBJECTS = \
"CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.o"

# External object files for target app-pingpong
app__pingpong_EXTERNAL_OBJECTS =

teshsuite/msg/app-pingpong/app-pingpong: teshsuite/msg/CMakeFiles/app-pingpong.dir/app-pingpong/app-pingpong.c.o
teshsuite/msg/app-pingpong/app-pingpong: teshsuite/msg/CMakeFiles/app-pingpong.dir/build.make
teshsuite/msg/app-pingpong/app-pingpong: lib/libsimgrid.so.3.21
teshsuite/msg/app-pingpong/app-pingpong: teshsuite/msg/CMakeFiles/app-pingpong.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable app-pingpong/app-pingpong"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app-pingpong.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/msg/CMakeFiles/app-pingpong.dir/build: teshsuite/msg/app-pingpong/app-pingpong

.PHONY : teshsuite/msg/CMakeFiles/app-pingpong.dir/build

teshsuite/msg/CMakeFiles/app-pingpong.dir/clean:
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -P CMakeFiles/app-pingpong.dir/cmake_clean.cmake
.PHONY : teshsuite/msg/CMakeFiles/app-pingpong.dir/clean

teshsuite/msg/CMakeFiles/app-pingpong.dir/depend:
	cd /home/kien/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/msg/CMakeFiles/app-pingpong.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/msg/CMakeFiles/app-pingpong.dir/depend

