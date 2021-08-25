# CMake generated Testfile for 
# Source directory: /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt
# Build directory: /home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(tesh-xbt-cmdline "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/cmdline" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/cmdline" "cmdline.tesh")
ADD_TEST(tesh-xbt-log_large "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/log_large" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/log_large" "log_large.tesh")
ADD_TEST(tesh-xbt-parallel_log_crashtest "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/parallel_log_crashtest" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/parallel_log_crashtest" "parallel_log_crashtest.tesh")
ADD_TEST(tesh-xbt-parmap_test "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/parmap_test" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/parmap_test" "parmap_test.tesh")
ADD_TEST(tesh-xbt-log "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/log_usage" "/home/aaa10078nj/fpga_network/SimGrid-3.21/teshsuite/xbt/log_usage/log_usage.tesh")
