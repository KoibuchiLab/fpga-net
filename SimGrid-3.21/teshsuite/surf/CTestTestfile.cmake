# CMake generated Testfile for 
# Source directory: /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf
# Build directory: /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(tesh-surf-lmm_usage "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/lmm_usage" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/lmm_usage" "lmm_usage.tesh")
ADD_TEST(tesh-surf-surf_usage "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/surf_usage" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/surf_usage" "surf_usage.tesh")
ADD_TEST(tesh-surf-surf_usage2 "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/surf_usage2" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/surf_usage2" "surf_usage2.tesh")
ADD_TEST(tesh-surf-maxmin-small "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "maxmin_bench_small.tesh")
ADD_TEST(tesh-surf-maxmin-medium "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "maxmin_bench_medium.tesh")
ADD_TEST(tesh-surf-maxmin-large "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "maxmin_bench_large.tesh")
