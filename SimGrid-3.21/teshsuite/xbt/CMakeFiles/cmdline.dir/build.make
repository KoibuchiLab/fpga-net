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
include teshsuite/xbt/CMakeFiles/cmdline.dir/depend.make

# Include the progress variables for this target.
include teshsuite/xbt/CMakeFiles/cmdline.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/xbt/CMakeFiles/cmdline.dir/flags.make

teshsuite/xbt/CMakeFiles/cmdline.dir/cmdline/cmdline.c.o: teshsuite/xbt/CMakeFiles/cmdline.dir/flags.make
teshsuite/xbt/CMakeFiles/cmdline.dir/cmdline/cmdline.c.o: teshsuite/xbt/cmdline/cmdline.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object teshsuite/xbt/CMakeFiles/cmdline.dir/cmdline/cmdline.c.o"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cmdline.dir/cmdline/cmdline.c.o   -c /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/cmdline/cmdline.c

teshsuite/xbt/CMakeFiles/cmdline.dir/cmdline/cmdline.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cmdline.dir/cmdline/cmdline.c.i"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/cmdline/cmdline.c > CMakeFiles/cmdline.dir/cmdline/cmdline.c.i

teshsuite/xbt/CMakeFiles/cmdline.dir/cmdline/cmdline.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cmdline.dir/cmdline/cmdline.c.s"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/cmdline/cmdline.c -o CMakeFiles/cmdline.dir/cmdline/cmdline.c.s

# Object files for target cmdline
cmdline_OBJECTS = \
"CMakeFiles/cmdline.dir/cmdline/cmdline.c.o"

# External object files for target cmdline
cmdline_EXTERNAL_OBJECTS =

teshsuite/xbt/cmdline/cmdline: teshsuite/xbt/CMakeFiles/cmdline.dir/cmdline/cmdline.c.o
teshsuite/xbt/cmdline/cmdline: teshsuite/xbt/CMakeFiles/cmdline.dir/build.make
teshsuite/xbt/cmdline/cmdline: lib/libsimgrid.so.3.21
teshsuite/xbt/cmdline/cmdline: teshsuite/xbt/CMakeFiles/cmdline.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cmdline/cmdline"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cmdline.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/xbt/CMakeFiles/cmdline.dir/build: teshsuite/xbt/cmdline/cmdline

.PHONY : teshsuite/xbt/CMakeFiles/cmdline.dir/build

teshsuite/xbt/CMakeFiles/cmdline.dir/clean:
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt && $(CMAKE_COMMAND) -P CMakeFiles/cmdline.dir/cmake_clean.cmake
.PHONY : teshsuite/xbt/CMakeFiles/cmdline.dir/clean

teshsuite/xbt/CMakeFiles/cmdline.dir/depend:
	cd /home/kien/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/CMakeFiles/cmdline.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/xbt/CMakeFiles/cmdline.dir/depend

