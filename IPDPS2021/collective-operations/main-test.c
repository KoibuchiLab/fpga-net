/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-12-19 17:38:28
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-01-08 21:55:02
 * @ Description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#ifdef __cplusplus
#include "kmpi.hpp"
#else
#include "kmpi.h"
#endif

#define RAND_SEED 721311

int main(int argc, char* argv[])
{
    int size, rank, NUM_ITEMS;
    char hostname[256];
    char topo[256];//for real MPICH test
    int hostname_len;


    MPI_Init(&argc, &argv);
    

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &hostname_len);
    KMPI_Init(rank, size);
    NUM_ITEMS = 500; //default numitems

    // Topology optional argument
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--num-item")) {
            if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%d", &NUM_ITEMS) != 1)) {
                printf("Invalid <topology> argument\n");
            }
        }
     }

    //	if (rank == 0) printf("Chunksize: %d\n", chunksize);
    float* sendbuf;
    if ((sendbuf = (float*)malloc(sizeof(float) * NUM_ITEMS)) == NULL) {
        printf("Out of memory!");
    }
    float* recvbuf;
    if ((recvbuf = (float*)malloc(sizeof(float) * NUM_ITEMS * size)) == NULL) {
        printf("Out of memory!");
    }
#if defined(DEBUG0)
    if (rank == 0) {
        printf("Network shape 1: %s\n", networkshape);
    }
#endif

    // All rank fill the buffer with random data
    srandom(RAND_SEED + rank);
    for (int j = 0; j < NUM_ITEMS; j++) {
        sendbuf[j] = (float)(1 + 1.0 * (random() % 100));
    }

    // Start the timer
    printf("Test MPI_Allgather\n");
    
    double start_time;
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    KMPI_Allgatherf(sendbuf, NUM_ITEMS, MPI_FLOAT, recvbuf, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
    
    /// Stop timer
    MPI_Barrier(MPI_COMM_WORLD);
    double kimrdtime = MPI_Wtime() - start_time;

    start_time = MPI_Wtime();
    float* resultlib = (float*)malloc(sizeof(float) * NUM_ITEMS * size);
    MPI_Allgather(sendbuf, NUM_ITEMS, MPI_FLOAT, resultlib, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if ((0 == rank)) {
        fprintf(stdout, "Time %.7lf,%.7lf,%d\n", kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
    }
    // Compare the result
    if (rank == 0) {
        for (int i = 0; i < NUM_ITEMS * size; i++) {
            if (recvbuf[i] != resultlib[i]) {
                fprintf(stdout, "%s, %s\n", "Result", "Allgather wrong");
                break;
            }
        }

    }
    
    float* alltoallsendbuf = (float*)malloc(sizeof(float) * NUM_ITEMS * size);
    for(int i = 0; i < NUM_ITEMS*size; i++){
        alltoallsendbuf[i] = i;
    }
    printf("Test MPI_Alltoall\n");
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    KMPI_Alltoallf(alltoallsendbuf, NUM_ITEMS, MPI_FLOAT, recvbuf, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);

    /// Stop timer
    MPI_Barrier(MPI_COMM_WORLD);
    kimrdtime = MPI_Wtime() - start_time;
    
    start_time = MPI_Wtime();
    MPI_Alltoall(alltoallsendbuf, NUM_ITEMS, MPI_FLOAT, resultlib, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if ((0 == rank)) {
        fprintf(stdout, "Time %.7lf,%.7lf,%d\n", kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
    }
    if (rank == 0) {
        for (int i = 0; i < NUM_ITEMS * size; i++) {
            if (recvbuf[i] != resultlib[i]) {
                fprintf(stdout, "%s, %s\n", "Result", "Allgather wrong");
                break;
            }
        }

    }

    float* allreducesendbuf = (float*)malloc(sizeof(float) * NUM_ITEMS);
    float* allreducerecvbuf = (float*)malloc(sizeof(float) * NUM_ITEMS);
    float* allreducerecvbuflib = (float*)malloc(sizeof(float) * NUM_ITEMS);
    for (int i = 0; i < NUM_ITEMS; i++) {
        alltoallsendbuf[i] = i;
    }
    printf("Test MPI_Allreduce\n");
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    KMPI_Allreducef(allreducesendbuf, allreducerecvbuf, NUM_ITEMS, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    /// Stop timer
    MPI_Barrier(MPI_COMM_WORLD);
    kimrdtime = MPI_Wtime() - start_time;

    start_time = MPI_Wtime();
    MPI_Allreduce(allreducesendbuf, allreducerecvbuflib, NUM_ITEMS, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if ((0 == rank)) {
        fprintf(stdout, "Time %.7lf,%.7lf,%d\n", kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
    }
    
    free(resultlib);
    MPI_Finalize();
    return 0;
}
