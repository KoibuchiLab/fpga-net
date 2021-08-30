# CMake generated Testfile for 
# Source directory: /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/mc
# Build directory: /home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/mc
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(mc-random-bug "/usr/bin/python3" "/home/kienpham/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/mc/random-bug" "--setenv" "srcdir=/home/kienpham/project/fpga_network/SimGrid-3.21" "--cd" "/home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/mc/random-bug" "random-bug.tesh")
set_tests_properties(mc-random-bug PROPERTIES  _BACKTRACE_TRIPLES "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;49;ADD_TEST;/home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/mc/CMakeLists.txt;42;ADD_TESH;/home/kienpham/project/fpga_network/SimGrid-3.21/teshsuite/mc/CMakeLists.txt;0;")
