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
include teshsuite/msg/CMakeFiles/get_sender.dir/depend.make

# Include the progress variables for this target.
include teshsuite/msg/CMakeFiles/get_sender.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/msg/CMakeFiles/get_sender.dir/flags.make

teshsuite/msg/CMakeFiles/get_sender.dir/get_sender/get_sender.c.o: teshsuite/msg/CMakeFiles/get_sender.dir/flags.make
teshsuite/msg/CMakeFiles/get_sender.dir/get_sender/get_sender.c.o: teshsuite/msg/get_sender/get_sender.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object teshsuite/msg/CMakeFiles/get_sender.dir/get_sender/get_sender.c.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/get_sender.dir/get_sender/get_sender.c.o   -c /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg/get_sender/get_sender.c

teshsuite/msg/CMakeFiles/get_sender.dir/get_sender/get_sender.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/get_sender.dir/get_sender/get_sender.c.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg/get_sender/get_sender.c > CMakeFiles/get_sender.dir/get_sender/get_sender.c.i

teshsuite/msg/CMakeFiles/get_sender.dir/get_sender/get_sender.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/get_sender.dir/get_sender/get_sender.c.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg/get_sender/get_sender.c -o CMakeFiles/get_sender.dir/get_sender/get_sender.c.s

# Object files for target get_sender
get_sender_OBJECTS = \
"CMakeFiles/get_sender.dir/get_sender/get_sender.c.o"

# External object files for target get_sender
get_sender_EXTERNAL_OBJECTS =

teshsuite/msg/get_sender/get_sender: teshsuite/msg/CMakeFiles/get_sender.dir/get_sender/get_sender.c.o
teshsuite/msg/get_sender/get_sender: teshsuite/msg/CMakeFiles/get_sender.dir/build.make
teshsuite/msg/get_sender/get_sender: lib/libsimgrid.so.3.21
teshsuite/msg/get_sender/get_sender: teshsuite/msg/CMakeFiles/get_sender.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable get_sender/get_sender"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/get_sender.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/msg/CMakeFiles/get_sender.dir/build: teshsuite/msg/get_sender/get_sender

.PHONY : teshsuite/msg/CMakeFiles/get_sender.dir/build

teshsuite/msg/CMakeFiles/get_sender.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg && $(CMAKE_COMMAND) -P CMakeFiles/get_sender.dir/cmake_clean.cmake
.PHONY : teshsuite/msg/CMakeFiles/get_sender.dir/clean

teshsuite/msg/CMakeFiles/get_sender.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/msg/CMakeFiles/get_sender.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/msg/CMakeFiles/get_sender.dir/depend

