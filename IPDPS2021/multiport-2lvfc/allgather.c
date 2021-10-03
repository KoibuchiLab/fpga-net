/*
 * allgather.c
 *
 *  Created on: Oct 2, 2021
 *      Author: kienpham
 */


/*
 * allgather.c
 *
 *  Created on: Sep 25, 2021
 *	  Author: kienpham
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "config.h"


#define RAND_SEED 721311

static void program_abort(char * message) {
	fprintf(stderr, "Error: %s", message);
}

int h2r(char *hostname, char *networkshape){ //host name to rank for 2lvfc
	int groupnumber, nodenumber;
	int networkshape_a, networkshape_b;

	sscanf(hostname, "%d_%d", &groupnumber, &nodenumber);
	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	return groupnumber * networkshape_b + nodenumber;
}

void r2h(int rank, char* networkshape, int *groupnumber, int *nodenumber){ // rank to host name for 2lvfc
	int networkshape_a, networkshape_b;
	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	*groupnumber = rank / networkshape_b;
	*nodenumber = rank % networkshape_b;
	return;
}

void r2h_r(int rank, char* topo, char* hostname){ // rank to host name for 2lvfc
	int numofgroups, numofnodes;
	sscanf(topo, "%dx%d", &numofgroups, &numofnodes);
	sprintf(hostname, "%d_%d", rank / numofnodes, rank % numofnodes);
	return;
}
static inline void swap(float *arr, int a, int b){
	float tmp = arr[a];
	arr[a] = arr[b];
	arr[b] = tmp;
	return;
}


int main(int argc, char *argv[])
{
	int size, rank, NUM_ITEMS;
	char hostname[256];
	char topo[256];//for real MPICH test
	int hostname_len;


	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname,&hostname_len);
	NUM_ITEMS = 500; //default numitems

	// Topology optional argument
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--topo")) {
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%s", topo) != 1)) {
				program_abort("Invalid <topology> argument\n");
			} else { // prepare fake hostname for testing on MPICH (without simulation
				r2h_r(rank, topo, hostname);
				hostname_len = strlen(hostname) + 1;
#if defined(DEBUG0)
				printf("Rank: %d | Host name: %s | Length: %d\n", rank, hostname, hostname_len);
#endif
			}
		}
		if (!strcmp(argv[i], "--num-item")) {
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%d", &NUM_ITEMS) != 1)) {
				program_abort("Invalid <topology> argument\n");
			}
		}

	}

//	if (rank == 0) printf("Chunksize: %d\n", chunksize);
	float * data;
	if ((data = malloc(sizeof(float) * NUM_ITEMS)) == NULL) {
		program_abort("Out of memory!");
	}
	char networkshape[256];
	int networkshape_len;
	if (rank == size - 1){
		strcpy(networkshape, hostname);
		networkshape_len = hostname_len + 1;
		networkshape[networkshape_len] = '\0';
	}
	MPI_Bcast(&networkshape_len, 1, MPI_INT, size - 1, MPI_COMM_WORLD);
	// Send the shape of the topology to all node
	if (MPI_SUCCESS != MPI_Bcast(networkshape, networkshape_len, MPI_CHAR, size - 1, MPI_COMM_WORLD)){
		program_abort("Broadcast networkshape fail\n");
	}
#if defined(DEBUG0)
	if (rank == 0){
		printf("Network shape 1: %s\n", networkshape);
	}
#endif

	// All rank fill the buffer with random data
	srandom(RAND_SEED + rank);
	for (int j = 0; j < NUM_ITEMS; j++) {
		data[j] = (float)(1 + 1.0 * (random() % 100));
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
	int groupnumber, nodenumber; // Hostname define in xml is groupnumber_nodenumber
	r2h(rank, networkshape, &groupnumber, &nodenumber);

	if (rank != h2r(hostname, networkshape))
		program_abort("Computed rank must equal to real rank\n");
	int numofgroups, numofnodesingroup;
	sscanf(networkshape, "%d_%d", &numofgroups, &numofnodesingroup);
	numofgroups++;
	numofnodesingroup++;

	 MPI_Request *reqrecvs, *reqsends;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLGATHER : START	  ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Step 1: Inter group gather  //////////////////////////////////////////////////////////////////
	// send inter group reduction result to other groups
	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	float *intergroupbuffer_ = (float*)malloc(sizeof(float)*numofgroups*NUM_ITEMS);
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	double dblasttimer = MPI_Wtime();
#endif
	for (int i = 0; i < numofgroups; i++){
		// Compute source
		int source = numofnodesingroup*i + nodenumber;
		if (source != rank){
			MPI_Irecv(&intergroupbuffer_[i*NUM_ITEMS], NUM_ITEMS, \
					MPI_FLOAT, source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (int i = 0; i < numofgroups; i++){
		// Compute destination
		int destination = numofnodesingroup*i + nodenumber;
		if (destination != rank){
			MPI_Isend(data, NUM_ITEMS, MPI_FLOAT, destination, 1, \
					MPI_COMM_WORLD, &reqsends[i]);
		} else {
			//intergroupbuffer_[i] <- data (data is on a process so dont need to send)
			for (int j = 0; j < NUM_ITEMS; j++){
				intergroupbuffer_[i*NUM_ITEMS + j] = data[j];
			}
		}
	}

	for (int i = 0; i < numofgroups; i++){
		// Compute source
		int source = numofnodesingroup*i + nodenumber;
		if (source != rank){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}

	for (int i = 0; i < numofgroups; i++){
		// Compute destination
		int destination = numofnodesingroup*i + nodenumber;
		if (destination != rank){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}

#if defined(DEBUG3)
	printf("From rank: %d|\t intergroubuffer_ (intergroup buffer for allgather)\n\t\t", rank);
	for(int i = 0; i < numofgroups*NUM_ITEMS; i++){
		printf("\t%.0f", intergroupbuffer_[i]);
	}
	printf("\n");
#endif
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for inter-group all-gather\n", MPI_Wtime() - dblasttimer);
	}
#endif
#if !defined(COMPARE_BUILDIN)
	free(data);
#endif
	free(reqsends);
	free(reqrecvs);
	// Step 2: Intra group gather  //////////////////////////////////////////////////////////////////
	//Final result
	float *allgatherresult = (float*)malloc(sizeof(float)*NUM_ITEMS*size);

	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofnodesingroup);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofnodesingroup);
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif

	for (int i = 0; i < NUM_ITEMS*size; i++){
		allgatherresult[i] = 0;
	}
	// Allocate buffer for intra group
	// Size = [numofnodesingroup][numofgroup*NUM_ITEMS]
	/*float **intragroupbuffer_ = (float**)malloc(sizeof(float*)*numofnodesingroup);
	for (int i = 0; i < numofnodesingroup; i++){
		intragroupbuffer_[i] = (float*)malloc(sizeof(float)*numofgroups*NUM_ITEMS);
		for (int j = 0; j < numofgroups*NUM_ITEMS; j++){
			intragroupbuffer_[i][j] = 0; // Should careful for other gather op
		}
	}*/

	for (int i = 0; i < numofnodesingroup; i++){
		int source = groupnumber*numofnodesingroup + i;
		if (rank != source){
			MPI_Irecv(&allgatherresult[i*numofgroups*NUM_ITEMS], numofgroups*NUM_ITEMS, MPI_FLOAT, \
					source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int destination = groupnumber*numofnodesingroup + i;
		if(rank != destination){
			MPI_Isend(intergroupbuffer_, numofgroups*NUM_ITEMS, MPI_FLOAT, \
					destination, 1, MPI_COMM_WORLD, &reqsends[i]);
		} else {
			for (int j = 0; j < numofgroups*NUM_ITEMS; j++){
				allgatherresult[i*numofgroups*NUM_ITEMS + j] = intergroupbuffer_[j];
			}
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int source = groupnumber*numofnodesingroup + i;
		if (rank != source){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	for (int i = 0; i < numofnodesingroup; i++){
		int destination = groupnumber*numofnodesingroup + i;
		if (rank != destination){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}
#if defined(DEBUG4)
	MPI_Barrier(MPI_COMM_WORLD);
	for(int i = 0; i < 100000; i++);
	if(rank == 0){
		printf("From rank %d, allgather buffer\n\t", rank);
		for(int i = 0; i < NUM_ITEMS*size; i++){
			printf("\t%.0f", allgatherresult[i]);
		}
	}

	printf("\n");
#endif

	// Allocate buffer for intra group
	// Size = [numofnodesingroup][numofgroup*numofitemkim
	float **intragroupbuffer_ = (float**)malloc(sizeof(float*)*numofnodesingroup);
	for (int i = 0; i < numofnodesingroup; i++){
		intragroupbuffer_[i] = (float*)malloc(sizeof(float)*numofgroups*NUM_ITEMS);
		for (int j = 0; j < numofgroups*NUM_ITEMS; j++){
			intragroupbuffer_[i][j] = 0; // Should careful for other gather op
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int source = groupnumber*numofnodesingroup + i;
		if (rank != source){
			MPI_Irecv(intragroupbuffer_[i], numofgroups*NUM_ITEMS, MPI_FLOAT, \
					source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int destination = groupnumber*numofnodesingroup + i;
		if(rank != destination){
			MPI_Isend(intergroupbuffer_, numofgroups*NUM_ITEMS, MPI_FLOAT, \
					destination, 1, MPI_COMM_WORLD, &reqsends[i]);
		} else {
			for (int j = 0; j < numofgroups*NUM_ITEMS; j++){
				intragroupbuffer_[i][j] = intergroupbuffer_[j];
			}
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int source = groupnumber*numofnodesingroup + i;
		if (rank != source){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
		for (int j = 0; j < NUM_ITEMS*numofgroups; j++){
			int index = j%NUM_ITEMS + i*NUM_ITEMS + (j/NUM_ITEMS)*numofnodesingroup*NUM_ITEMS;
			allgatherresult[index] = intragroupbuffer_[i][j];
		}
	}
	for (int i = 0; i < numofnodesingroup; i++){
		int destination = groupnumber*numofnodesingroup + i;
		if (rank != destination){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}
	free(intergroupbuffer_);

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for intra-group allgather\n", MPI_Wtime() - dblasttimer);
	}
#endif
#if defined(DEBUG4)
	MPI_Barrier(MPI_COMM_WORLD);
	for(int i = 0; i < 100000; i++);
	printf("From rank %d, allgather result\n\t", rank);
	for(int i = 0; i < NUM_ITEMS*size; i++){
		printf("\t%.0f", allgatherresult[i]);
	}
	printf("\n");
#endif

	/// Stop timer
	MPI_Barrier(MPI_COMM_WORLD);
	double kimrdtime = MPI_Wtime() - start_time;

	free(reqrecvs);
	free(reqsends);

#if defined(COMPARE_BUILDIN)
	start_time = MPI_Wtime();
	float *allgatherresultlib = (float*)malloc(sizeof(float)*NUM_ITEMS*size);
	MPI_Allgather(data, NUM_ITEMS, MPI_FLOAT, allgatherresultlib, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "%s,%.7lf,%.7lf,%d\n", networkshape, kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
	}

	// Compare the result
	if (rank == 0){
	for (int i = 0; i < NUM_ITEMS*size; i++){
		if (allgatherresult[i] != allgatherresultlib[i]){
			fprintf(stdout, "%s, %s\n", networkshape, "Allgather wrong");
			break;
		}
	}
	free(data);
#endif

#if defined(DEBUG5)
		printf("Kim allgather: \n");
		for (int i = 0; i < NUM_ITEMS*size; i++){
			printf(" %.0f", allgatherresult[i]);
		}
		printf("\nLib Allgather\n");
		for (int i = 0; i < NUM_ITEMS*size; i++){
			printf(" %.0f", allgatherresultlib[i]);
		}
		printf("\n");
#endif

#if defined(COMPARE_BUILDIN)
	}
	free(allgatherresultlib);
#endif
	free(allgatherresult);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	   ALLGATHER : END	   ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	MPI_Finalize();
	return 0;
}
