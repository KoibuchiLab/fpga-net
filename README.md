# fpga_network
Simulate FPGA indirect network

# Build Simgrid
The installation requires the cpp compiler and fortran compiler (for fortran application). Use below command to find the existed compiler in your system
`dpkg --list | grep compiler`

**Install compiler**
```
# sudo apt-itude install flex bison
# sudo apt-get install gfortran
```
< May require the MPI install for MPI parallel application >

**Prepare Cmake file**
```
# cd simgrid_data_redundancy/
# mkdir build     % Create folder for building objects and execution file. 
# cmake -DCMAKE_INSTALL_PREFIX=./build -Denable_compile_optimizations=on -Denable_smpi=on -Denable_documentation=off
```
**Edit CmakeList for python installation**
Which python or which python3
```
set(PYTHON_EXECUTABLE /apps/python/3.6.5/bin)
set(PYTHON_INCLUDE_DIR /apps/python/3.6.5/include/)
set(PYTHON_LIBRARY /usr/lib/libpython3.so)
```

**Make and install**
```
# make
# make install
```
