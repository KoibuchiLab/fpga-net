/*
 * reducescatter.c
 *
 *  Created on: Sep 25, 2021
 *      Author: kienpham
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

//#define DEBUG 0
//#define DEBUG1 // Intra group reduscatter
//#define DEBUG2   // Inter group reducescatter
//#define DEBUG3   // Inter group allgather
//#define DEBUG4   // Intra group allgather


#define RAND_SEED 721311

static void program_abort(char * message) {
	fprintf(stderr, "Error: %s", message);
}

int h2r(char *hostname_r, char *networkshape_r){ //host name to rank for 2lvfc
	char hostname[256], networkshape[256];
	strcpy(hostname, hostname_r);
	strcpy(networkshape, networkshape_r);
	char *token = strtok(hostname, "_");
	int groupnumber = atoi(token);
	token = strtok(NULL, "_");
	int nodenumber = atoi(token);

	token = strtok(networkshape, "_");
	token = strtok(NULL, "_");
	int networkshape_b = atoi(token) + 1;
	return groupnumber * networkshape_b + nodenumber;
}

void r2h(int rank, char* networkshape_r, int *groupnumber, int *nodenumber){ // rank to host name for 2lvfc
	char networkshape[256];
	strcpy(networkshape, networkshape_r);
	char * token = strtok(networkshape, "_");
	token = strtok(NULL, "_");
	int networkshape_b = atoi(token) + 1;
	*groupnumber = rank / networkshape_b;
	*nodenumber = rank % networkshape_b;
	return;
}

float reduce(float a, float b, const char *op){
	if (strcmp(op, "sum") == 0){
		return a + b;
	} else {
		return 0;
	}
}

int main(int argc, char *argv[])
{
	int size, rank, NUM_ITEMS;
	struct timeval start, end;
	char hostname[256];
	int hostname_len;


	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname,&hostname_len);
	if( argc > 1){
		NUM_ITEMS = atoi(argv[1]);
	} else {
		NUM_ITEMS = 500;
	}

//	if (rank == 0) printf("Chunksize: %d\n", chunksize);
	int NUM_ITEMS_ROUND = NUM_ITEMS - NUM_ITEMS%size;
	float * data;
	if ((data = malloc(sizeof(float) * NUM_ITEMS_ROUND)) == NULL) {
		program_abort("Out of memory!");
	}
	char networkshape[256];
	if (rank == size - 1){
		strcpy(networkshape, hostname);
	}
	// Send the shape of the topology to all node
	if (MPI_SUCCESS != MPI_Bcast(networkshape, hostname_len, MPI_CHAR, size - 1, MPI_COMM_WORLD)){
		program_abort("Broadcast networkshape fail\n");
	}



	// All rank fill the buffer with random data
	srandom(RAND_SEED + rank);
	for (int j = 0; j < NUM_ITEMS_ROUND; j++) {
		data[j] = (float)(1 + 1.0 * (random() % 8));
	}

#if defined(DEBUG1)
	printf("Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS_ROUND; i++){
		printf("%.0f\t", data[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i <1000000; i++);
#endif

	
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "%s,%.7lf,%d\n", networkshape, MPI_Wtime() - start_time, NUM_ITEMS);
	}
	float *allreduceresultlib = (float*)malloc(sizeof(float)*NUM_ITEMS_ROUND);
	MPI_Allreduce(data, allreduceresultlib, NUM_ITEMS_ROUND, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
/*
	if (rank == 0){
		for (int i = 0; i < NUM_ITEMS_ROUND; i++){
			if (allreduceresult[i] != allreduceresultlib[i]){
				fprintf(stdout, "%s, %s\n", networkshape, "Allreduce wrong");
			}
		}
		printf("Kim allreduce: \n");
		for (int i = 0; i < NUM_ITEMS_ROUND; i++){
			printf(" %.0f", allreduceresult[i]);
		}
		printf("\nLib Allreduce\n");
		for (int i = 0; i < NUM_ITEMS_ROUND; i++){
			printf(" %.0f", allreduceresultlib[i]);
		}
		printf("\n");
	}*/

	free(allreduceresultlib);
	free(allreduceresult);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////       ALLGATHER : END       ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	MPI_Finalize();
	return 0;
}

