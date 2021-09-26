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
#define DEBUG1

#define NUM_ITEMS 6//

#define RAND_SEED 72131

static void program_abort(char * message) {
	int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, & my_rank);
	if (my_rank == 0) {
		if (message) {
			fprintf(stderr, "Error: %s", message);
		}
	}
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

int main(int argc, char *argv[])
{
	int size, rank;
	struct timeval start, end;
	char hostname[256];
	int hostname_len;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname,&hostname_len);

//	if (rank == 0) printf("Chunksize: %d\n", chunksize);
	float * data;
	if ((data = malloc(sizeof(float) * NUM_ITEMS)) == NULL) {
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
	for (int j = 0; j < NUM_ITEMS; j++) {
		data[j] = (float)(1 + 1.0 * (random() % 8));
	}

#if defined(DEBUG1)
	printf("Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS; i++){
		printf("%.1f\t", data[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i <1000000; i++);
#endif

	// Start the timer
	double start_time;
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		start_time = MPI_Wtime();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////   REDUCE - SCATTER : BEGIN  ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int groupnumber, nodenumber; // Hostname define in xml is groupnumber_nodenumber
	r2h(rank, networkshape, &groupnumber, &nodenumber);
	printf("Host name %s \t| rank %d \t| rank computed %d\t| %d_%d\n", hostname, rank, h2r(hostname, networkshape), \
			groupnumber, nodenumber);
	if (rank != h2r(hostname, networkshape))
		program_abort("Computed rank must equal to real rank\n");

	char networkshapetmp[256];
	strcpy(networkshapetmp, networkshape);
	char *token = strtok(networkshapetmp, "_");
	int numofgroup = atoi(token) + 1;
	token = strtok(NULL, "_");
	int numofnodesingroup = atoi(token) + 1;
	//Step 1: Intra group exchange

	int numofitemsineachchunk = NUM_ITEMS / numofnodesingroup;

	//Prepare buffer to receive data
	float* intragroupbuffer = (float*)malloc(sizeof(float)*numofnodesingroup*numofitemsineachchunk);
	for(int i = 0; i < numofnodesingroup*numofitemsineachchunk; i++){
		intragroupbuffer[i] = 0;
	}

	MPI_Request * reqsends = (MPI_Request*) malloc(sizeof(MPI_Request)*numofnodesingroup);
	MPI_Request * reqrecvs = (MPI_Request*) malloc(sizeof(MPI_Request)*numofnodesingroup);
#if defined(DEBUG1)
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i <1000000; i++);
	printf("Initial buffer from rank %d: ", rank);
	for (int i = 0; i < numofnodesingroup*numofitemsineachchunk; i++){
		printf("%.1f\t", intragroupbuffer[i]);
	}
	printf("\n");
#endif

	// Place MPI_Irecv
	for (int i = 0; i < numofnodesingroup; i++){
		// Compute source
		int source = numofnodesingroup*groupnumber + i;
		if (source != rank){
			MPI_Irecv(&intragroupbuffer[i*numofitemsineachchunk], numofitemsineachchunk, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}
	// Place MPI_Isend
	for (int i = 0; i < numofnodesingroup; i++){
		// Compute destination
		int destination = numofnodesingroup*groupnumber + i;
		if (destination != rank){
			MPI_Isend(&data[i*numofitemsineachchunk], numofitemsineachchunk, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);
		}
	}

	//wait for nonblocking receive to complete
	for (int i = 0; i < numofnodesingroup; i++){
		// Compute destination
		int source = numofnodesingroup*groupnumber + i;
		if (source != rank){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	//wait for nonblocking send to complete
	for (int i = 0; i < numofnodesingroup; i++){
		// Compute destination
		int destination = numofnodesingroup*groupnumber + i;
		if (destination != rank){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}
#if defined(DEBUG1)
	printf("Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS; i++){
		printf("%.1f\t", data[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i <1000000; i++);
	printf("Reduce scatter from rank %d: ", rank);
	for (int i = 0; i < numofnodesingroup*numofitemsineachchunk; i++){
		printf("%.1f\t", intragroupbuffer[i]);
	}
	printf("\n");
#endif
	free(intragroupbuffer);
	free(reqsends);
	free(reqrecvs);
	//Step 2: Inter group exchange


	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////   REDUCE - SCATTER : END    ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/// Stop timer
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "time: %.3lf seconds\n",
			MPI_Wtime() - start_time);
	}
	// Check the result
	int chunksize = NUM_ITEMS/size;
	// MPI Lib result
	int *libresult = (int*) malloc(sizeof(int) * chunksize);
	int *counts = (int*) malloc(sizeof(int) * size);
	for (int i = 0; i < size; i++){
		counts[i] = (int)chunksize;
	}
	MPI_Reduce_scatter(data, libresult, counts, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
#if defined(DEBUG)
	printf("Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS; i++){
		printf("%d\t", data[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for(int i = 0; i < 1000000; i++);
	fflush(stdout);

	printf("Reduce result from rank %d: ", rank);
	for(int i = 0; i < chunksize; i++){
		printf("%d\t", libresult[i]);
	}
	printf("\n");
#endif

/*	int libresult = -1;
	int *counts = (int*)malloc(sizeof(int) * NUM_ITEMS);
	for (int i = 0; i < NUM_ITEMS; i++){
		counts[i] = 1;
	}
	MPI_Reduce_scatter(data, &libresult, counts, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
#if defined(DEBUG)
	printf("Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS; i++){
		printf("%d\t", data[i]);
	}
	printf("\n");
	fflush(stdout);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("Reduce result from rank %d: %d\n", rank, libresult);

#endif*/

	free(libresult);
	free(counts);
	free(data);
	MPI_Barrier(MPI_COMM_WORLD);
	for(int i = 0; i < 1000000; i++);
	if (rank == 0){ printf("\n");printf("\n");}
	MPI_Finalize();
	return 0;
}

