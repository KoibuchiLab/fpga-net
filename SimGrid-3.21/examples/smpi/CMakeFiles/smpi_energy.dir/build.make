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
include examples/smpi/CMakeFiles/smpi_energy.dir/depend.make

# Include the progress variables for this target.
include examples/smpi/CMakeFiles/smpi_energy.dir/progress.make

# Include the compile flags for this target's objects.
include examples/smpi/CMakeFiles/smpi_energy.dir/flags.make

examples/smpi/CMakeFiles/smpi_energy.dir/energy/energy.c.o: examples/smpi/CMakeFiles/smpi_energy.dir/flags.make
examples/smpi/CMakeFiles/smpi_energy.dir/energy/energy.c.o: examples/smpi/energy/energy.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/smpi/CMakeFiles/smpi_energy.dir/energy/energy.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/smpi_energy.dir/energy/energy.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/energy/energy.c

examples/smpi/CMakeFiles/smpi_energy.dir/energy/energy.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/smpi_energy.dir/energy/energy.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/energy/energy.c > CMakeFiles/smpi_energy.dir/energy/energy.c.i

examples/smpi/CMakeFiles/smpi_energy.dir/energy/energy.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/smpi_energy.dir/energy/energy.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi && /home/kienpham/project/fpga_network/SimGrid-3.21/smpi_script/bin/smpicc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/energy/energy.c -o CMakeFiles/smpi_energy.dir/energy/energy.c.s

# Object files for target smpi_energy
smpi_energy_OBJECTS = \
"CMakeFiles/smpi_energy.dir/energy/energy.c.o"

# External object files for target smpi_energy
smpi_energy_EXTERNAL_OBJECTS =

examples/smpi/energy/smpi_energy: examples/smpi/CMakeFiles/smpi_energy.dir/energy/energy.c.o
examples/smpi/energy/smpi_energy: examples/smpi/CMakeFiles/smpi_energy.dir/build.make
examples/smpi/energy/smpi_energy: lib/libsimgrid.so.3.21
examples/smpi/energy/smpi_energy: examples/smpi/CMakeFiles/smpi_energy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable energy/smpi_energy"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/smpi_energy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/smpi/CMakeFiles/smpi_energy.dir/build: examples/smpi/energy/smpi_energy

.PHONY : examples/smpi/CMakeFiles/smpi_energy.dir/build

examples/smpi/CMakeFiles/smpi_energy.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi && $(CMAKE_COMMAND) -P CMakeFiles/smpi_energy.dir/cmake_clean.cmake
.PHONY : examples/smpi/CMakeFiles/smpi_energy.dir/clean

examples/smpi/CMakeFiles/smpi_energy.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/CMakeFiles/smpi_energy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/smpi/CMakeFiles/smpi_energy.dir/depend

