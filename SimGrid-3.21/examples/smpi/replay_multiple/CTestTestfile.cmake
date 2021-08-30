# CMake generated Testfile for 
# Source directory: /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple
# Build directory: /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(smpi-replay-multiple "/usr/bin/python3" "/home/kienpham/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "srcdir=/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple" "--setenv" "bindir=/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple" "--cd" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple/replay_multiple.tesh")
set_tests_properties(smpi-replay-multiple PROPERTIES  _BACKTRACE_TRIPLES "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;49;ADD_TEST;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple/CMakeLists.txt;6;ADD_TESH;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/replay_multiple/CMakeLists.txt;0;")
