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
include examples/smpi/NAS/CMakeFiles/dt.dir/depend.make

# Include the progress variables for this target.
include examples/smpi/NAS/CMakeFiles/dt.dir/progress.make

# Include the compile flags for this target's objects.
include examples/smpi/NAS/CMakeFiles/dt.dir/flags.make

examples/smpi/NAS/CMakeFiles/dt.dir/dt.c.o: examples/smpi/NAS/CMakeFiles/dt.dir/flags.make
examples/smpi/NAS/CMakeFiles/dt.dir/dt.c.o: examples/smpi/NAS/dt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/smpi/NAS/CMakeFiles/dt.dir/dt.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dt.dir/dt.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/dt.c

examples/smpi/NAS/CMakeFiles/dt.dir/dt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dt.dir/dt.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/dt.c > CMakeFiles/dt.dir/dt.c.i

examples/smpi/NAS/CMakeFiles/dt.dir/dt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dt.dir/dt.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/dt.c -o CMakeFiles/dt.dir/dt.c.s

examples/smpi/NAS/CMakeFiles/dt.dir/nas_common.c.o: examples/smpi/NAS/CMakeFiles/dt.dir/flags.make
examples/smpi/NAS/CMakeFiles/dt.dir/nas_common.c.o: examples/smpi/NAS/nas_common.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object examples/smpi/NAS/CMakeFiles/dt.dir/nas_common.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dt.dir/nas_common.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/nas_common.c

examples/smpi/NAS/CMakeFiles/dt.dir/nas_common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dt.dir/nas_common.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/nas_common.c > CMakeFiles/dt.dir/nas_common.c.i

examples/smpi/NAS/CMakeFiles/dt.dir/nas_common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dt.dir/nas_common.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/nas_common.c -o CMakeFiles/dt.dir/nas_common.c.s

examples/smpi/NAS/CMakeFiles/dt.dir/DGraph.c.o: examples/smpi/NAS/CMakeFiles/dt.dir/flags.make
examples/smpi/NAS/CMakeFiles/dt.dir/DGraph.c.o: examples/smpi/NAS/DGraph.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object examples/smpi/NAS/CMakeFiles/dt.dir/DGraph.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/dt.dir/DGraph.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/DGraph.c

examples/smpi/NAS/CMakeFiles/dt.dir/DGraph.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dt.dir/DGraph.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/DGraph.c > CMakeFiles/dt.dir/DGraph.c.i

examples/smpi/NAS/CMakeFiles/dt.dir/DGraph.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dt.dir/DGraph.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/DGraph.c -o CMakeFiles/dt.dir/DGraph.c.s

# Object files for target dt
dt_OBJECTS = \
"CMakeFiles/dt.dir/dt.c.o" \
"CMakeFiles/dt.dir/nas_common.c.o" \
"CMakeFiles/dt.dir/DGraph.c.o"

# External object files for target dt
dt_EXTERNAL_OBJECTS =

examples/smpi/NAS/dt: examples/smpi/NAS/CMakeFiles/dt.dir/dt.c.o
examples/smpi/NAS/dt: examples/smpi/NAS/CMakeFiles/dt.dir/nas_common.c.o
examples/smpi/NAS/dt: examples/smpi/NAS/CMakeFiles/dt.dir/DGraph.c.o
examples/smpi/NAS/dt: examples/smpi/NAS/CMakeFiles/dt.dir/build.make
examples/smpi/NAS/dt: lib/libsimgrid.so.3.21
examples/smpi/NAS/dt: examples/smpi/NAS/CMakeFiles/dt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable dt"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/smpi/NAS/CMakeFiles/dt.dir/build: examples/smpi/NAS/dt

.PHONY : examples/smpi/NAS/CMakeFiles/dt.dir/build

examples/smpi/NAS/CMakeFiles/dt.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS && $(CMAKE_COMMAND) -P CMakeFiles/dt.dir/cmake_clean.cmake
.PHONY : examples/smpi/NAS/CMakeFiles/dt.dir/clean

examples/smpi/NAS/CMakeFiles/dt.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/NAS/CMakeFiles/dt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/smpi/NAS/CMakeFiles/dt.dir/depend

