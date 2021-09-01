# CMake generated Testfile for 
# Source directory: /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf
# Build directory: /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(tesh-surf-lmm_usage "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/lmm_usage" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/lmm_usage" "lmm_usage.tesh")
add_test(tesh-surf-surf_usage "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/surf_usage" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/surf_usage" "surf_usage.tesh")
add_test(tesh-surf-surf_usage2 "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/surf_usage2" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/surf_usage2" "surf_usage2.tesh")
add_test(tesh-surf-maxmin-small "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "maxmin_bench_small.tesh")
add_test(tesh-surf-maxmin-medium "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "maxmin_bench_medium.tesh")
add_test(tesh-surf-maxmin-large "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "platfdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/platforms" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/surf/maxmin_bench" "maxmin_bench_large.tesh")
