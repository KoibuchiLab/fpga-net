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
include teshsuite/smpi/CMakeFiles/bug-17132.dir/depend.make

# Include the progress variables for this target.
include teshsuite/smpi/CMakeFiles/bug-17132.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/smpi/CMakeFiles/bug-17132.dir/flags.make

teshsuite/smpi/CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.o: teshsuite/smpi/CMakeFiles/bug-17132.dir/flags.make
teshsuite/smpi/CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.o: teshsuite/smpi/bug-17132/bug-17132.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object teshsuite/smpi/CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi/bug-17132/bug-17132.c

teshsuite/smpi/CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi/bug-17132/bug-17132.c > CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.i

teshsuite/smpi/CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi/bug-17132/bug-17132.c -o CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.s

# Object files for target bug-17132
bug__17132_OBJECTS = \
"CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.o"

# External object files for target bug-17132
bug__17132_EXTERNAL_OBJECTS =

teshsuite/smpi/bug-17132/bug-17132: teshsuite/smpi/CMakeFiles/bug-17132.dir/bug-17132/bug-17132.c.o
teshsuite/smpi/bug-17132/bug-17132: teshsuite/smpi/CMakeFiles/bug-17132.dir/build.make
teshsuite/smpi/bug-17132/bug-17132: lib/libsimgrid.so.3.21
teshsuite/smpi/bug-17132/bug-17132: teshsuite/smpi/CMakeFiles/bug-17132.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bug-17132/bug-17132"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bug-17132.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/smpi/CMakeFiles/bug-17132.dir/build: teshsuite/smpi/bug-17132/bug-17132

.PHONY : teshsuite/smpi/CMakeFiles/bug-17132.dir/build

teshsuite/smpi/CMakeFiles/bug-17132.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi && $(CMAKE_COMMAND) -P CMakeFiles/bug-17132.dir/cmake_clean.cmake
.PHONY : teshsuite/smpi/CMakeFiles/bug-17132.dir/clean

teshsuite/smpi/CMakeFiles/bug-17132.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/smpi/CMakeFiles/bug-17132.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/smpi/CMakeFiles/bug-17132.dir/depend

