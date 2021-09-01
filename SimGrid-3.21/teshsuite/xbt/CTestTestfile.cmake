# CMake generated Testfile for 
# Source directory: /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt
# Build directory: /home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(tesh-xbt-cmdline "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/cmdline" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/cmdline" "cmdline.tesh")
add_test(tesh-xbt-log_large "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/log_large" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/log_large" "log_large.tesh")
add_test(tesh-xbt-parallel_log_crashtest "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parallel_log_crashtest" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parallel_log_crashtest" "parallel_log_crashtest.tesh")
add_test(tesh-xbt-parmap_test "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--setenv" "bindir=/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parmap_test" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/parmap_test" "parmap_test.tesh")
add_test(tesh-xbt-log "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/log_usage" "/home/kien/project/fpga_network/SimGrid-3.21/teshsuite/xbt/log_usage/log_usage.tesh")
