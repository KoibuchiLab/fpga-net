#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])  {
    int numtasks, rank, dest, source, rc, count,len;  
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);
    // allocate new size for send buf
    MPI_Request *reqsends, *reqrecvs;
    int NUM_ITEMS = 2, d = 4;
    // float *sendbuf2 = new float[NUM_ITEMS*d];//(float*)malloc(sizeof(float)*NUM_ITEMS*d);
    float **recvbufv2 = (float**)malloc(sizeof(float*)*(d));
    for (int i = 0; i < d; i++){
        recvbufv2[i] = (float*)malloc(sizeof(float)*d*NUM_ITEMS);
    }
    reqsends = new MPI_Request[d];
    reqrecvs = new MPI_Request[d];

    delete [] reqrecvs;
    delete [] reqsends;
    // delete sendbuf2;
    for (int i = 1; i < d; i++){
        free(recvbufv2[i]);
    }
    free(recvbufv2);
    MPI_Finalize();
}