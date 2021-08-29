# CMake generated Testfile for 
# Source directory: /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f90
# Build directory: /home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f90
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
ADD_TEST(smpi-energy-f90-thread "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:thread" "--setenv" "srcdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy" "/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f90/energy.tesh")
ADD_TEST(smpi-energy-f90-ucontext "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:ucontext" "--setenv" "srcdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy" "/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f90/energy.tesh")
ADD_TEST(smpi-energy-f90-raw "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:raw" "--setenv" "srcdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy" "/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f90/energy.tesh")
ADD_TEST(smpi-energy-f90-boost "/home/aaa10078nj/.local/bin/python3" "/home/aaa10078nj/fpga_network/SimGrid-3.21/bin/tesh" "--ignore-jenkins" "--cfg" "contexts/factory:boost" "--setenv" "srcdir=/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy" "--cd" "/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy" "/home/aaa10078nj/fpga_network/SimGrid-3.21/examples/smpi/energy/f90/energy.tesh")
