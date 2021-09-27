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
//#define DEBUG1
#define DEBUG2

#define NUM_ITEMS 18//

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
		printf("%.0f\t", data[i]);
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
#if defined(DEBUG0)
	printf("Host name %s \t| rank %d \t| rank computed %d\t| %d_%d\n", hostname, rank, h2r(hostname, networkshape), \
			groupnumber, nodenumber);
#endif
	if (rank != h2r(hostname, networkshape))
		program_abort("Computed rank must equal to real rank\n");

	char networkshapetmp[256];
	strcpy(networkshapetmp, networkshape);
	char *token = strtok(networkshapetmp, "_");
	int numofgroups = atoi(token) + 1;
	token = strtok(NULL, "_");
	int numofnodesingroup = atoi(token) + 1;

	//Step 1: Intra group exchange  /////////////////////////////////////////////////////////////////

	int numofitemsineachchunk = NUM_ITEMS / numofnodesingroup;

	//Prepare buffer to receive data
	float ** intragroupbuffer = (float**)malloc(sizeof(float*)*numofnodesingroup);
	for (int i = 0; i < numofnodesingroup; i++){
		intragroupbuffer[i] = (float*)malloc(sizeof(float)*numofitemsineachchunk);
		for(int j = 0; j < numofitemsineachchunk; j++){
			intragroupbuffer[i][j] = 0;
		}
	}

	MPI_Request * reqsends = (MPI_Request*) malloc(sizeof(MPI_Request)*numofnodesingroup);
	MPI_Request * reqrecvs = (MPI_Request*) malloc(sizeof(MPI_Request)*numofnodesingroup);


	// Place MPI_Irecv
	for (int i = 0; i < numofnodesingroup; i++){
		// Compute source
		int source = numofnodesingroup*groupnumber + i;
		if (source != rank){
			MPI_Irecv(intragroupbuffer[i], numofitemsineachchunk, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}
	// Place MPI_Isend
	for (int i = 0; i < numofnodesingroup; i++){
		// Compute destination
		int destination = numofnodesingroup*groupnumber + i;
		if (destination != rank){
			MPI_Isend(&data[i*numofitemsineachchunk], numofitemsineachchunk, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);
		} else {
			// copy data reduction buffer (intragroupbuffer)
			for (int j = 0; j < numofitemsineachchunk; j++){
				intragroupbuffer[i][j] = data[i*numofitemsineachchunk + j];
			}
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
		printf("%.0f\t", data[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i <1000000; i++);
	printf("Intra group Reduce scatter from rank %d: \n", rank);
	for (int i = 0; i < numofnodesingroup; i++){
		printf("\t\t");
		for (int j = 0; j < numofitemsineachchunk; j++){
			printf("%.0f\t", intragroupbuffer[i][j]);
		}
		printf("\n");
	}
	printf("\n");
#endif

	// execute the reduction operation
	float *intragroupreductionresult = (float*)malloc(sizeof(float)*numofitemsineachchunk);
	for (int i = 0; i < numofitemsineachchunk; i++){
		intragroupreductionresult[i] = 0;
	}
	for (int i = 0; i < numofnodesingroup; i++){
		for (int j = 0; j < numofitemsineachchunk; j++){
			intragroupreductionresult[j] = reduce(intragroupreductionresult[j], intragroupbuffer[i][j], "sum");
		}
	}
#if defined(DEBUG2)
	printf("From rank %d | Intra group reduction result:\t", rank);
	for (int i = 0; i < numofitemsineachchunk; i++){
		printf("%.0f\t", intragroupreductionresult[i]);
	}
	printf("\n");
#endif
#if defined(DEBUG1)
	printf("Intra group reduction result:\t");
	for (int i = 0; i < numofitemsineachchunk; i++){
		printf("%.0f\t", intragroupreductionresult[i]);
	}
	printf("\n");
#endif

	// free memory
	for(int i = 0; i < numofnodesingroup; i++){
		free(intragroupbuffer[i]);
	}
	free(intragroupbuffer);
	free(reqsends);
	free(reqrecvs);



	//Step 2: Inter group exchange  /////////////////////////////////////////////////////////////////
	//Prepare buffer to receive data
	int numofitemsinsecondreduction = NUM_ITEMS / size;
	float * intergroupbuffer = (float*)malloc(sizeof(float)*numofgroups*numofitemsinsecondreduction);

	reqsends = (MPI_Request*) malloc(sizeof(MPI_Request)*numofgroups*numofitemsinsecondreduction);
	reqrecvs = (MPI_Request*) malloc(sizeof(MPI_Request)*numofgroups*numofitemsinsecondreduction);

	// Place MPI_Irecv
	for (int i = 0; i < numofgroups; i++){
		// Compute source
		int source = numofnodesingroup*i + nodenumber;
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			if (source != rank){
				MPI_Irecv(&intergroupbuffer[j*numofgroups + i], 1, \
						MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[j*numofgroups + i]);
			}
		}

	}
	// Place MPI_Isend
	for (int i = 0; i < numofgroups; i++){
		// Compute destination
		int destination = numofnodesingroup*i + nodenumber;
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			if (destination != rank){
				MPI_Isend(&intragroupreductionresult[j*numofgroups + i], \
						1, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[j*numofgroups + i]);
			} else {
				// copy data reduction buffer (intragroupbuffer)
				intergroupbuffer[j*numofgroups + i] \
						= intragroupreductionresult[j*numofgroups + i];
			}
		}

	}

	// Wait for MPI_Irecv to complete
	for (int i = 0; i < numofgroups; i++){
		// Compute source
		int source = numofnodesingroup*i + nodenumber;
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			if(source != rank){
				MPI_Wait(&reqrecvs[j*numofgroups + i], MPI_STATUS_IGNORE);
			}
		}
	}

	// Wait for MPI_Isend to complete
	for (int i = 0; i < numofgroups; i++){
		// Compute source
		int destination = numofnodesingroup*i + nodenumber;
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			if(destination != rank){
				MPI_Wait(&reqsends[j*numofgroups + i], MPI_STATUS_IGNORE);
			}
		}
	}
#if defined(DEBUG2)
	printf("From rank %d | Inter group buffer: ", rank);
	for (int i = 0; i < numofgroups*numofitemsinsecondreduction; i++){
		printf("\t%.0f", intergroupbuffer[i]);
	}
	printf("\n");
#endif
	//Execute reduction
	float *intergroupreductionresult = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);
	for (int i = 0; i < numofitemsinsecondreduction; i++){
		intergroupreductionresult[i] = 0; // Should careful for reduction operation = multiply
	}
	for (int i = 0; i < numofgroups; i++){
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			intergroupreductionresult[j] = reduce( intergroupbuffer[j*numofgroups + i],\
					intergroupreductionresult[j], "sum");
		}
	}

#if defined(DEBUG2)
	MPI_Barrier(MPI_COMM_WORLD);
	for(int i = 0; i < 1000000; i++);
	printf("From rank %d, inter group reduction result: ", rank);
	for (int i = 0; i < numofitemsinsecondreduction; i++){
		printf("\t%.0f", intergroupreductionresult[i]);
	}
	printf("\n");
#endif

	free(intergroupbuffer);
	free(intragroupreductionresult);
	free(intergroupreductionresult);
	free(reqsends);
	free(reqrecvs);
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

