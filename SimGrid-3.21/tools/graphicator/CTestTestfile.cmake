# CMake generated Testfile for 
# Source directory: /home/kienpham/project/fpga_network/SimGrid-3.21/tools/graphicator
# Build directory: /home/kienpham/project/fpga_network/SimGrid-3.21/tools/graphicator
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(graphicator "/usr/bin/python3" "/home/kienpham/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "srcdir=/home/kienpham/project/fpga_network/SimGrid-3.21" "--setenv" "bindir=/home/kienpham/project/fpga_network/SimGrid-3.21/bin" "--cd" "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/graphicator" "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/graphicator/graphicator.tesh")
set_tests_properties(graphicator PROPERTIES  _BACKTRACE_TRIPLES "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;49;ADD_TEST;/home/kienpham/project/fpga_network/SimGrid-3.21/tools/graphicator/CMakeLists.txt;4;ADD_TESH;/home/kienpham/project/fpga_network/SimGrid-3.21/tools/graphicator/CMakeLists.txt;0;")
