#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <smpi/smpi.h>

//#define N (1024 * 1024 * 1)
//#define N (1024 * 1024 * 32)  //Weight size
//#define K 8 	//Batch size
//#define M 1	//Number of iteration
int main(int argc, char *argv[])
{
  int size, rank;
  struct timeval start, end;
  char hostname[256];
  int hostname_len;
  int messsge_size = 1024 * 1024;
  int message_number = 1;
  
  gettimeofday(&start,NULL);
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Get_processor_name(hostname,&hostname_len);

  messsge_size = atoi(argv[1]) * messsge_size / sizeof(double);
  message_number = atoi(argv[2]);
  if (rank == 0) {
	  printf("Message size %d, (%d bytes)\n",messsge_size,sizeof(double) * messsge_size);  
	  printf("Message number %d\n",message_number);  
  }
  int N = messsge_size;
  int M =message_number;  
  double comm_time =0;
  //TYPE: MPI_DOUBLE_PRECISION, MPI_REAL, MPI_INT, MPI_BYTE

  // Allocate a 1MiB buffer
	double *local_sum = malloc(sizeof(double) * N);
	double *global_sum = malloc(sizeof(double) * N);
	int j=0;
	for (j=0; j < N; j++){
		//local_sum[j] = (N-j) * (rank +1);
		//local_sum[j] = rank * N + j;
		local_sum[j] = j;
	}
	
	if (rank == 0) {
		fprintf(stderr,"%s: %d/%d,buffer: %d, iteration: %d\n", hostname, rank, size, N, M);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	gettimeofday(&end,NULL);
	if (rank == 0) {
		comm_time = (end.tv_sec*1000000.0 + end.tv_usec - start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0;
		printf("Start communication at %f\n",comm_time);  
	}
	int i=0;
	for (i = 0; i < M; i++){
		//For each interation
		//printf("Local sum for rank %d - %.10e, iteration: %d\n", rank, local_sum[0],M);

		//Allreduce the local_sum to the global_sum
		MPI_Allreduce(local_sum, global_sum, N, MPI_DOUBLE_PRECISION, MPI_SUM,MPI_COMM_WORLD);
		
		// Print the result
		//printf("Global sum in rank %d - %.10e\n", rank, global_sum[N-1]);
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	if (rank == 0) {
		gettimeofday(&end,NULL);
		printf("End communication at %f\n",(end.tv_sec*1000000.0 + end.tv_usec - start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0);
		comm_time = (end.tv_sec*1000000.0 + end.tv_usec - start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0 - comm_time;
		printf("Communication in\t%f\n",comm_time);
	}
	MPI_Finalize();
	return 0;
}
