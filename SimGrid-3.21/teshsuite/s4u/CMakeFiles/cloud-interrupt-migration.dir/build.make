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
include teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/depend.make

# Include the progress variables for this target.
include teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/progress.make

# Include the compile flags for this target's objects.
include teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/flags.make

teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.o: teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/flags.make
teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.o: teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.o"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.o -c /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration.cpp

teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.i"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration.cpp > CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.i

teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.s"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration.cpp -o CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.s

# Object files for target cloud-interrupt-migration
cloud__interrupt__migration_OBJECTS = \
"CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.o"

# External object files for target cloud-interrupt-migration
cloud__interrupt__migration_EXTERNAL_OBJECTS =

teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration: teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/cloud-interrupt-migration/cloud-interrupt-migration.cpp.o
teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration: teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/build.make
teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration: lib/libsimgrid.so.3.21
teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration: teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kien/project/fpga_network/SimGrid-3.21/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cloud-interrupt-migration/cloud-interrupt-migration"
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cloud-interrupt-migration.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/build: teshsuite/s4u/cloud-interrupt-migration/cloud-interrupt-migration

.PHONY : teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/build

teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/clean:
	cd /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u && $(CMAKE_COMMAND) -P CMakeFiles/cloud-interrupt-migration.dir/cmake_clean.cmake
.PHONY : teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/clean

teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/depend:
	cd /home/kien/project/fpga_network/SimGrid-3.21 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u /home/kien/project/fpga_network/SimGrid-3.21 /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : teshsuite/s4u/CMakeFiles/cloud-interrupt-migration.dir/depend

