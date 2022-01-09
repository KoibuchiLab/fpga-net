/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-12-19 19:42:02
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-01-09 16:59:25
 * @ Description:
 */


#ifndef IPDPS2021_KMPI_HPP_
#define IPDPS2021_KMPI_HPP_

#include <vector>
#include <iostream>
#include <cstdlib>
#include <list>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cmath>
#include <string>
#include <mpi.h>
#include <stdio.h>
#include "../../simgrid-3.28/install/include/smpi/smpi.h"

 // size 2d + 1; last element is duplicate index


using namespace std;
struct Int3 {
    int dst;
    int src;
    int sendidx;
    int recvidx;
};

static vector <int>* childParent;
static vector <Int3> scheduleTable;


int h2r(const char* hostname, int degree);
int hidx2r(const int a, const int b, int degree);
void r2h(int rank, int degree, int& a, int& b);
void r2h_r(int rank, int degree, char* hostname);
void program_abort(const char* message);
int KMPI_Allgatherf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
    float* recvbuf, int recvcount, MPI_Datatype recvtype,
    MPI_Comm comm);

int KMPI_Alltoallf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
    float* recvbuf, int recvcount, MPI_Datatype recvtype,
    MPI_Comm comm);

int KMPI_Allreducef(const float* sendbuf, float* recvbuf, size_t count,
    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);

int KMPI_Init(int rank, int size);

#endif /* IPDPS2021_KMPI_HPP_ */