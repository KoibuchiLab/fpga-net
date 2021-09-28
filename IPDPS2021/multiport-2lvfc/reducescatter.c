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

//#define NUM_ITEMS 180000//
#define CHUNK_SIZE 100

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
	int NUM_ITEMS = CHUNK_SIZE * size;
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
	free(reqsends);
	free(reqrecvs);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////   REDUCE - SCATTER : END    ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////      ALLGATHER : START      ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Step 1: Inter group gather  //////////////////////////////////////////////////////////////////
	// send inter group reduction result to other groups
	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	/*float **intergroupbuffer_ = (float**)malloc(sizeof(float*)*numofgroups);
	for (int i = 0; i < numofgroups; i++){
		intergroupbuffer_[i] = (float*) malloc(sizeof(float)*numofitemsinsecondreduction);
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			intergroupbuffer_[i][j] = 0; // Should careful for reduction operation = multiply
		}
	}*/
	float *intergroupbuffer_ = (float*)malloc(sizeof(float)*numofgroups*numofitemsinsecondreduction);

	for (int i = 0; i < numofgroups; i++){
		// Compute source
		int source = numofnodesingroup*i + nodenumber;
		if (source != rank){
			MPI_Irecv(&intergroupbuffer_[i*numofitemsinsecondreduction], numofitemsinsecondreduction, \
					MPI_FLOAT, source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (int i = 0; i < numofgroups; i++){
		// Compute destination
		int destination = numofnodesingroup*i + nodenumber;
		if (destination != rank){
			MPI_Isend(intergroupreductionresult, numofitemsinsecondreduction, MPI_FLOAT, destination, 1, \
					MPI_COMM_WORLD, &reqsends[i]);
		} else {
			//intergroupbuffer_[i] <- intergroupreductionresult (data is on a process so dont need to send)
			for (int j = 0; j < numofitemsinsecondreduction; j++){
				intergroupbuffer_[i*numofitemsinsecondreduction + j] = intergroupreductionresult[j];
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
	for(int i = 0; i < numofgroups*numofitemsinsecondreduction; i++){
		printf("\t%.0f", intergroupbuffer_[i]);
	}
	printf("\n");
#endif

	free(intergroupreductionresult);
	// Step 2: Intra group gather  //////////////////////////////////////////////////////////////////
	//Final result
	float *allreduceresult = (float*)malloc(sizeof(float)*NUM_ITEMS);

	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofnodesingroup);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofnodesingroup);

	for (int i = 0; i < NUM_ITEMS; i++){
		allreduceresult[i] = 0;
	}
	//Allocate buffer for intra group
	float **intragroupbuffer_ = (float**)malloc(sizeof(float*)*numofnodesingroup);
	for (int i = 0; i < numofnodesingroup; i++){
		intragroupbuffer_[i] = (float*)malloc(sizeof(float)*numofgroups*numofitemsinsecondreduction);
		for (int j = 0; j < numofgroups*numofitemsinsecondreduction; j++){
			intragroupbuffer_[i][j] = 0; // Should careful for other reduce op
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int source = groupnumber*numofnodesingroup + i;
		if (rank != source){
			MPI_Irecv(intragroupbuffer_[i], numofgroups*numofitemsinsecondreduction, MPI_FLOAT, \
					source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int destination = groupnumber*numofnodesingroup + i;
		if(rank != destination){
			MPI_Isend(intergroupbuffer_, numofgroups*numofitemsinsecondreduction, MPI_FLOAT, \
					destination, 1, MPI_COMM_WORLD, &reqsends[i]);
		} else {
			for (int j = 0; j < numofgroups*numofitemsinsecondreduction; j++){
				intragroupbuffer_[i][j] = intergroupbuffer_[j];
			}
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int source = groupnumber*numofnodesingroup + i;
		if (rank != source){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			for (int j = 0; j < numofitemsinsecondreduction; j++){
				for(int k = 0; k < numofgroups; k++){
					allreduceresult[i*numofgroups + j*size + k] = intragroupbuffer_[i][j*numofgroups + k];
				}
			}

		} else { //copy from local buffer -> intergroupbuffer_
			for (int j = 0; j < numofitemsinsecondreduction; j++){
				for(int k = 0; k < numofgroups; k++){
					allreduceresult[i*numofgroups + j*size + k] = intergroupbuffer_[j*numofgroups + k];
				}
			}
		}
	}
	for (int i = 0; i < numofnodesingroup; i++){
		int destination = groupnumber*numofnodesingroup + i;
		if (rank != destination){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}
#if defined(DEBUG4)
	printf("From rank %d|\t, intra group buffer_\n", rank);
	for (int i = 0; i < numofnodesingroup; i++){
		printf("\t\t");
		for (int j = 0; j < numofgroups*numofitemsinsecondreduction; j ++){
			printf("\t%.0f", intragroupbuffer_[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for(int i = 0; i < 100000; i++);
	printf("From rank %d, allreduce result\n\t", rank);
	for(int i = 0; i < NUM_ITEMS; i++){
		printf("\t%.0f", allreduceresult[i]);
	}
	printf("\n");
#endif
	free(intergroupbuffer_);
	free(reqrecvs);
	free(reqsends);
	free(intragroupbuffer_);
	free(allreduceresult);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////       ALLGATHER : END       ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/// Stop timer
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "%.7lf\n", MPI_Wtime() - start_time);
	}
	MPI_Finalize();
	return 0;
}

