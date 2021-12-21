/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-12-19 19:42:02
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-12-21 22:28:41
 * @ Description:
 */

#include <mpi.h>
#ifndef IPDPS2021_KMPI_H_
#define IPDPS2021_KMPI_H_

int KMPI_Allgatherf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
    float* recvbuf, int recvcount, MPI_Datatype recvtype,
    MPI_Comm comm);

int KMPI_Alltoallf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
    float* recvbuf, int recvcount, MPI_Datatype recvtype,
    MPI_Comm comm);

#endif /* IPDPS2021_KMPI_H_ */