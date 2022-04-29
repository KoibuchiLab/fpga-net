#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int size, rank;
    size_t bufsize = 256;
    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--num-item")) {
            if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%ld", &bufsize) != 1)) {
                printf("Invalid num of items.\n");
            }
        }
    }

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    
    float* sendbuf = (float*)malloc(sizeof(float)*bufsize);
    float* recvbuf = (float*)malloc(sizeof(float)*bufsize);
    MPI_Request send, recv;

    if (size == 1) MPI_Finalize();
    if (rank == 0){
        MPI_Irecv(recvbuf, bufsize, MPI_FLOAT, 2, 0, MPI_COMM_WORLD, &recv);
        MPI_Isend(sendbuf, bufsize, MPI_FLOAT, 2, 0, MPI_COMM_WORLD, &send);

        // MPI_Send(sendbuf, bufsize, MPI_FLOAT, 2, 0, MPI_COMM_WORLD);
        // MPI_Recv(sendbuf, bufsize, MPI_FLOAT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    } 
    if (rank == 2) {
        MPI_Irecv(recvbuf, bufsize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &recv);
        MPI_Isend(sendbuf, bufsize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &send);
        // MPI_Recv(recvbuf, bufsize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // MPI_Send(sendbuf, bufsize, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Wait(&send, MPI_STATUS_IGNORE);
    MPI_Wait(&recv, MPI_STATUS_IGNORE);

    MPI_Finalize();
}
