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
include examples/s4u/CMakeFiles/s4u-energy-boot.dir/depend.make

# Include the progress variables for this target.
include examples/s4u/CMakeFiles/s4u-energy-boot.dir/progress.make

# Include the compile flags for this target's objects.
include examples/s4u/CMakeFiles/s4u-energy-boot.dir/flags.make

examples/s4u/CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.o: examples/s4u/CMakeFiles/s4u-energy-boot.dir/flags.make
examples/s4u/CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.o: examples/s4u/energy-boot/s4u-energy-boot.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/s4u/CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.o"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.o -c /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u/energy-boot/s4u-energy-boot.cpp

examples/s4u/CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.i"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u/energy-boot/s4u-energy-boot.cpp > CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.i

examples/s4u/CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.s"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u/energy-boot/s4u-energy-boot.cpp -o CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.s

# Object files for target s4u-energy-boot
s4u__energy__boot_OBJECTS = \
"CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.o"

# External object files for target s4u-energy-boot
s4u__energy__boot_EXTERNAL_OBJECTS =

examples/s4u/energy-boot/s4u-energy-boot: examples/s4u/CMakeFiles/s4u-energy-boot.dir/energy-boot/s4u-energy-boot.cpp.o
examples/s4u/energy-boot/s4u-energy-boot: examples/s4u/CMakeFiles/s4u-energy-boot.dir/build.make
examples/s4u/energy-boot/s4u-energy-boot: lib/libsimgrid.so.3.21
examples/s4u/energy-boot/s4u-energy-boot: examples/s4u/CMakeFiles/s4u-energy-boot.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kienpham/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable energy-boot/s4u-energy-boot"
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/s4u-energy-boot.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/s4u/CMakeFiles/s4u-energy-boot.dir/build: examples/s4u/energy-boot/s4u-energy-boot

.PHONY : examples/s4u/CMakeFiles/s4u-energy-boot.dir/build

examples/s4u/CMakeFiles/s4u-energy-boot.dir/clean:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u && $(CMAKE_COMMAND) -P CMakeFiles/s4u-energy-boot.dir/cmake_clean.cmake
.PHONY : examples/s4u/CMakeFiles/s4u-energy-boot.dir/clean

examples/s4u/CMakeFiles/s4u-energy-boot.dir/depend:
	cd /home/kienpham/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u /home/kienpham/project/fpga_network/SimGrid-3.21 /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u /home/kienpham/project/fpga_network/SimGrid-3.21/examples/s4u/CMakeFiles/s4u-energy-boot.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/s4u/CMakeFiles/s4u-energy-boot.dir/depend

