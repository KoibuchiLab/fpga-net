# CMake generated Testfile for 
# Source directory: /home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy/f77
# Build directory: /home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy/f77
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(smpi-energy-f77-thread "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:thread" "--setenv" "srcdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy" "/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy/f77/energy.tesh")
add_test(smpi-energy-f77-ucontext "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:ucontext" "--setenv" "srcdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy" "/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy/f77/energy.tesh")
add_test(smpi-energy-f77-raw "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:raw" "--setenv" "srcdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy" "/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy/f77/energy.tesh")
add_test(smpi-energy-f77-boost "/usr/bin/python3" "/home/kien/project/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:boost" "--setenv" "srcdir=/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy" "--cd" "/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy" "/home/kien/project/fpga_network/SimGrid-3.21/examples/smpi/energy/f77/energy.tesh")
