# CMake generated Testfile for 
# Source directory: /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave
# Build directory: /home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(smpi-msg-masterslave-thread "/usr/bin/python3" "/home/kienpham/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:thread" "--setenv" "srcdir=/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave" "--cd" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/msg_smpi.tesh")
set_tests_properties(smpi-msg-masterslave-thread PROPERTIES  _BACKTRACE_TRIPLES "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;49;ADD_TEST;/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;64;ADD_TESH;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/CMakeLists.txt;6;ADD_TESH_FACTORIES;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/CMakeLists.txt;0;")
add_test(smpi-msg-masterslave-ucontext "/usr/bin/python3" "/home/kienpham/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:ucontext" "--setenv" "srcdir=/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave" "--cd" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/msg_smpi.tesh")
set_tests_properties(smpi-msg-masterslave-ucontext PROPERTIES  _BACKTRACE_TRIPLES "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;49;ADD_TEST;/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;64;ADD_TESH;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/CMakeLists.txt;6;ADD_TESH_FACTORIES;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/CMakeLists.txt;0;")
add_test(smpi-msg-masterslave-raw "/usr/bin/python3" "/home/kienpham/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:raw" "--setenv" "srcdir=/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave" "--cd" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/msg_smpi.tesh")
set_tests_properties(smpi-msg-masterslave-raw PROPERTIES  _BACKTRACE_TRIPLES "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;49;ADD_TEST;/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;64;ADD_TESH;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/CMakeLists.txt;6;ADD_TESH_FACTORIES;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/CMakeLists.txt;0;")
add_test(smpi-msg-masterslave-boost "/usr/bin/python3" "/home/kienpham/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:boost" "--setenv" "srcdir=/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave" "--cd" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave" "/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/msg_smpi.tesh")
set_tests_properties(smpi-msg-masterslave-boost PROPERTIES  _BACKTRACE_TRIPLES "/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;49;ADD_TEST;/home/kienpham/project/fpga_network/SimGrid-3.21/tools/cmake/Tests.cmake;64;ADD_TESH;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/CMakeLists.txt;6;ADD_TESH_FACTORIES;/home/kienpham/project/fpga_network/SimGrid-3.21/examples/smpi/smpi_msg_masterslave/CMakeLists.txt;0;")
