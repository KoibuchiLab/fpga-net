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
include teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/depend.make

# Include the progress variables for this target.
include teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/flags.make

teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.o: teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/flags.make
teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.o: teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.o"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/kien/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.o   -c /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong.c

teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.i"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/kien/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong.c > CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.i

teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.s"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/kien/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong.c -o CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.s

# Object files for target pt2pt-pingpong
pt2pt__pingpong_OBJECTS = \
"CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.o"

# External object files for target pt2pt-pingpong
pt2pt__pingpong_EXTERNAL_OBJECTS =

teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong: teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/pt2pt-pingpong/pt2pt-pingpong.c.o
teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong: teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/build.make
teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong: lib/libsimgrid.so.3.21
teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong: teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable pt2pt-pingpong/pt2pt-pingpong"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pt2pt-pingpong.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/build: teshsuite/smpi/pt2pt-pingpong/pt2pt-pingpong

.PHONY : teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/build

teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/clean:
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi && $(CMAKE_COMMAND) -P CMakeFiles/pt2pt-pingpong.dir/cmake_clean.cmake
.PHONY : teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/clean

teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/depend:
	cd /home/kien/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/smpi/CMakeFiles/pt2pt-pingpong.dir/depend

