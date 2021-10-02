/*
 * reducescatter.c
 *
 *  Created on: Sep 25, 2021
 *	  Author: kienpham
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

#include "config.h"

static void program_abort(char * message) {
	fprintf(stderr, "Error: %s", message);
}

unsigned int h2r(char *hostname, char *networkshape){ //host name to rank for 2lvfc
	unsigned int groupnumber, nodenumber;
	unsigned int networkshape_a, networkshape_b;

	sscanf(hostname, "%d_%d", &groupnumber, &nodenumber);
	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	return groupnumber * networkshape_b + nodenumber;
}

void r2h(unsigned int rank, char* networkshape, unsigned int *groupnumber, unsigned int *nodenumber){ // rank to host name for 2lvfc
	unsigned int networkshape_a, networkshape_b;
	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	*groupnumber = rank / networkshape_b;
	*nodenumber = rank % networkshape_b;
	return;
}

void r2h_r(unsigned int rank, char* topo, char* hostname){ // rank to host name for 2lvfc
	unsigned int numofgroups, numofnodes;
	sscanf(topo, "%dx%d", &numofgroups, &numofnodes);
	sprintf(hostname, "%d_%d", rank / numofnodes, rank % numofnodes);
	return;
}
static inline float reduce(const float a,const float b, const char *op){
	if (op[0] == 's'){ // sum
		return a + b;
	} else {
		return 0;
	}
}

unsigned int main(unsigned int argc, char *argv[])
{
	unsigned int size, rank, NUM_ITEMS;
	struct timeval start, end;
	char hostname[256];
	char topo[256];//for real MPICH test
	unsigned int hostname_len;


	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname,&hostname_len);
	NUM_ITEMS = 500; //default numitems

	// Topology optional argument
	for (unsigned int i = 1; i < argc; i++) {
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
	unsigned int NUM_ITEMS_ROUND = NUM_ITEMS - NUM_ITEMS%size;
	float * data;
	if ((data = malloc(sizeof(float) * NUM_ITEMS_ROUND)) == NULL) {
		program_abort("Out of memory!");
	}
	char networkshape[256];
	unsigned int networkshape_len;
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
	for (unsigned int j = 0; j < NUM_ITEMS_ROUND; j++) {
		data[j] = (float)(1 + 1.0 * (random() % 9));
	}
	float sum = 0;
	if (rank == 0){
		for (int i = 0; i < NUM_ITEMS_ROUND; i++){
			sum += data[i];
		}
	}
#if defined(DEBUG1)
	printf("Data from rank %d: ", rank);
	for (unsigned int i = 0; i < NUM_ITEMS_ROUND; i++){
		printf("%.0f\t", data[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for (unsigned int i = 0; i <1000000; i++);
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
	unsigned int groupnumber, nodenumber; // Hostname define in xml is groupnumber_nodenumber
	r2h(rank, networkshape, &groupnumber, &nodenumber);

	if (rank != h2r(hostname, networkshape))
		program_abort("Computed rank must equal to real rank\n");
	unsigned int numofgroups, numofnodesingroup;
	sscanf(networkshape, "%d_%d", &numofgroups, &numofnodesingroup);
	numofgroups++;
	numofnodesingroup++;

#if defined(DEBUG0)
	printf("Host name %s \t| rank %d \t| rank computed %d\t| %d_%d\n", hostname, rank, h2r(hostname, networkshape), \
			groupnumber, nodenumber);
	if(rank == 0){
		printf("Network shape: %s\n", networkshape);
	}
#endif

	//Step 1: Intra group exchange  /////////////////////////////////////////////////////////////////
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	double dblasttimer = MPI_Wtime();
#endif
	unsigned int numofitemsineachchunk = NUM_ITEMS_ROUND / numofnodesingroup;

	//Prepare buffer to receive data
	float ** intragroupbuffer = (float**)malloc(sizeof(float*)*numofnodesingroup);
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		intragroupbuffer[i] = (float*)malloc(sizeof(float)*numofitemsineachchunk);
		for(unsigned int j = 0; j < numofitemsineachchunk; j++){
			intragroupbuffer[i][j] = 0;
		}
	}

	MPI_Request * reqsends = (MPI_Request*) malloc(sizeof(MPI_Request)*numofnodesingroup);
	MPI_Request * reqrecvs = (MPI_Request*) malloc(sizeof(MPI_Request)*numofnodesingroup);


	// Place MPI_Irecv
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		// Compute source
		unsigned int source = numofnodesingroup*groupnumber + i;
		if (source != rank){
			MPI_Irecv(intragroupbuffer[i], numofitemsineachchunk, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}
	// Place MPI_Isend
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		// Compute destination
		unsigned int destination = numofnodesingroup*groupnumber + i;
		if (destination != rank){
			MPI_Isend(&data[i*numofitemsineachchunk], numofitemsineachchunk, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);
		} else {
			// copy data reduction buffer (intragroupbuffer)
			for (unsigned int j = 0; j < numofitemsineachchunk; j++){
				intragroupbuffer[i][j] = data[i*numofitemsineachchunk + j];
			}
		}
	}

	//wait for nonblocking receive to complete
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		// Compute destination
		unsigned int source = numofnodesingroup*groupnumber + i;
		if (source != rank){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	//wait for nonblocking send to complete
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		// Compute destination
		unsigned int destination = numofnodesingroup*groupnumber + i;
		if (destination != rank){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}

#if defined(DEBUG1)
	MPI_Barrier(MPI_COMM_WORLD);
	for (unsigned int i = 0; i <1000000; i++);
	printf("Intra group Reduce scatter from rank %d: \n", rank);
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		printf("\t\t");
		for (unsigned int j = 0; j < numofitemsineachchunk; j++){
			printf("%.0f\t", intragroupbuffer[i][j]);
		}
		printf("\n");
	}
	printf("\n");
#endif

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for intra-group reduce scatter communication time\n", MPI_Wtime() - dblasttimer);
	}
#endif

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif

	// execute the reduction operation
	float *intragroupreductionresult = (float*)malloc(sizeof(float)*numofitemsineachchunk);
	for (unsigned int i = 0; i < numofitemsineachchunk; i++){
		intragroupreductionresult[i] = 0;
	}
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		for (unsigned int j = 0; j < numofitemsineachchunk; j++){
			intragroupreductionresult[j] = reduce(intragroupreductionresult[j], intragroupbuffer[i][j], "sum");
		}
	}



#if defined(DEBUG2)
	printf("From rank %d | Intra group reduction result:\t", rank);
	for (unsigned int i = 0; i < numofitemsineachchunk; i++){
		printf("%.0f\t", intragroupreductionresult[i]);
	}
	printf("\n");
#endif

	// free memory
	for(unsigned int i = 0; i < numofnodesingroup; i++){
		free(intragroupbuffer[i]);
		;
	}
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for intra-group reduce scatter computation time\n", MPI_Wtime() - dblasttimer);
	}
#endif
	free(intragroupbuffer);
	free(reqsends);
	free(reqrecvs);



	//Step 2: Inter group exchange  /////////////////////////////////////////////////////////////////
	//Prepare buffer to receive data
	unsigned int numofitemsinsecondreduction = NUM_ITEMS_ROUND / size;

	reqsends = (MPI_Request*) malloc(sizeof(MPI_Request)*numofgroups);
	reqrecvs = (MPI_Request*) malloc(sizeof(MPI_Request)*numofgroups);
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif

	// Place MPI_Irecv
	float **recvbuf = (float**)malloc(sizeof(float*)*numofgroups);
	for (unsigned int i = 0; i < numofgroups; i++){
		recvbuf[i] = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);
	}
	for (unsigned int i = 0; i < numofgroups; i++){
		// Compute source
		unsigned int source = numofnodesingroup*i + nodenumber;
		if (source != rank){
			MPI_Irecv(recvbuf[i], numofitemsinsecondreduction, MPI_FLOAT, source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}/*
		for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
			if (source != rank){
				MPI_Irecv(&intergroupbuffer[j*numofgroups + i], 1, \
						MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[j*numofgroups + i]);
			}
		}*/

	}
	// Place MPI_Isend
	float *sendbuf = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);


	for (unsigned int i = 0; i < numofgroups; i++){
		// Compute destination
		unsigned int destination = numofnodesingroup*i + nodenumber;

		// Prepare buffer
		if (destination != rank){
			for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
				sendbuf[j] = intragroupreductionresult[j*numofgroups + i];
			}
			MPI_Isend(sendbuf, numofitemsinsecondreduction, MPI_FLOAT, destination, 1, MPI_COMM_WORLD, &reqsends[i]);
		} else { //Data is available, just copy it
			for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
				recvbuf[i][j] = intragroupreductionresult[j*numofgroups + i];
			}
		}

		/*for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
			if (destination != rank){
				MPI_Isend(&intragroupreductionresult[j*numofgroups + i], \
						1, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[j*numofgroups + i]);
			} else {
				// copy data reduction buffer (intragroupbuffer)
				intergroupbuffer[j*numofgroups + i] \
						= intragroupreductionresult[j*numofgroups + i];
			}
		}*/
	}
	free(sendbuf);

#if defined(DEBUG2)
	for (unsigned int i = 0; i < numofgroups; i++){
		unsigned int source = numofnodesingroup*i + nodenumber;
		if (source != rank){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	printf("From rank %d: recvbuf:\n", rank);
	for (unsigned int i = 0; i < numofgroups; i++){
		printf("\t");
		for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
			printf("\t%.0f", recvbuf[i][j]);
		}
		printf("\n");
	}
#endif

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif
	//If receive a buffer do reduction, and free the memory. Can improve by MPI_Test
	float *intergroupreductionresult = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);
	for (unsigned int i = 0; i < numofitemsinsecondreduction; i++){
		intergroupreductionresult[i] = 0; // Should careful for reduction operation = multiply
	}
	for (unsigned int i = 0; i < numofgroups; i++){
		// Compute source
		unsigned int source = numofnodesingroup*i + nodenumber;
		if(source != rank){

#if !defined(DEBUG2)
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
#endif
			for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
				intergroupreductionresult[j] = reduce(recvbuf[i][j], intergroupreductionresult[j], "sum");
			}
			//free(recvbuf[i]);
		} else {
			for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
				intergroupreductionresult[j] = reduce(recvbuf[i][j], intergroupreductionresult[j], "sum");
			}
			//free(recvbuf[i]);
		}
	}
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for inter-group reduce scatter computation time\n", MPI_Wtime() - dblasttimer);
	}
#endif



	// Wait for MPI_Isend to complete
	for (unsigned int i = 0; i < numofgroups; i++){
		// Compute source
		unsigned int destination = numofnodesingroup*i + nodenumber;
		if(destination != rank){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for inter-group reduce scatter communication time\n", MPI_Wtime() - dblasttimer);
	}
#endif



#if defined(DEBUG2)
	MPI_Barrier(MPI_COMM_WORLD);
	for(unsigned int i = 0; i < 1000000; i++);
	printf("From rank %d, inter group reduction result: ", rank);
	for (unsigned int i = 0; i < numofitemsinsecondreduction; i++){
		printf("\t%.0f", intergroupreductionresult[i]);
	}
	printf("\n");
#endif
	for (unsigned int i = 0; i < numofgroups; i++){
		free(recvbuf[i]);
	}
	free(recvbuf);
	free(intragroupreductionresult);
	free(reqsends);
	free(reqrecvs);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////   REDUCE - SCATTER : END	////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLGATHER : START	  ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Step 1: Inter group gather  //////////////////////////////////////////////////////////////////
	// send inter group reduction result to other groups
	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	float *intergroupbuffer_ = (float*)malloc(sizeof(float)*numofgroups*numofitemsinsecondreduction);
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif
	for (unsigned int i = 0; i < numofgroups; i++){
		// Compute source
		unsigned int source = numofnodesingroup*i + nodenumber;
		if (source != rank){
			MPI_Irecv(&intergroupbuffer_[i*numofitemsinsecondreduction], numofitemsinsecondreduction, \
					MPI_FLOAT, source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (unsigned int i = 0; i < numofgroups; i++){
		// Compute destination
		unsigned int destination = numofnodesingroup*i + nodenumber;
		if (destination != rank){
			MPI_Isend(intergroupreductionresult, numofitemsinsecondreduction, MPI_FLOAT, destination, 1, \
					MPI_COMM_WORLD, &reqsends[i]);
		} else {
			//intergroupbuffer_[i] <- intergroupreductionresult (data is on a process so dont need to send)
			for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
				intergroupbuffer_[i*numofitemsinsecondreduction + j] = intergroupreductionresult[j];
			}
		}
	}

	for (unsigned int i = 0; i < numofgroups; i++){
		// Compute source
		unsigned int source = numofnodesingroup*i + nodenumber;
		if (source != rank){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	for (unsigned int i = 0; i < numofgroups; i++){
		// Compute destination
		unsigned int destination = numofnodesingroup*i + nodenumber;
		if (destination != rank){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}
	free(reqsends);
	free(reqrecvs);
#if defined(DEBUG3)
	printf("From rank: %d|\t intergroubuffer_ (intergroup buffer for allgather)\n\t\t", rank);
	for(unsigned int i = 0; i < numofgroups*numofitemsinsecondreduction; i++){
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
	MPI_Barrier(MPI_COMM_WORLD);
	free(intergroupreductionresult);
	// Step 2: Intra group gather  //////////////////////////////////////////////////////////////////
	//Final result
	float *allreduceresult = (float*)malloc(sizeof(float)*NUM_ITEMS_ROUND);

	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofnodesingroup);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofnodesingroup);
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif

	for (unsigned int i = 0; i < NUM_ITEMS_ROUND; i++){
		allreduceresult[i] = 0;
	}
	//Allocate buffer for intra group

	float **intragroupbuffer_ = (float**)malloc(sizeof(float*)*numofnodesingroup);
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		intragroupbuffer_[i] = (float*)malloc(sizeof(float)*numofgroups*numofitemsinsecondreduction);
		for (unsigned int j = 0; j < numofgroups*numofitemsinsecondreduction; j++){
			intragroupbuffer_[i][j] = 0; // Should careful for other reduce op
		}
	}

	for (unsigned int i = 0; i < numofnodesingroup; i++){
		unsigned int source = groupnumber*numofnodesingroup + i;
		if (rank != source){
			MPI_Irecv(intragroupbuffer_[i], numofgroups*numofitemsinsecondreduction, MPI_FLOAT, \
					source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (unsigned int i = 0; i < numofnodesingroup; i++){
		unsigned int destination = groupnumber*numofnodesingroup + i;
		if(rank != destination){
			MPI_Isend(intergroupbuffer_, numofgroups*numofitemsinsecondreduction, MPI_FLOAT, \
					destination, 1, MPI_COMM_WORLD, &reqsends[i]);
		} else {
			for (unsigned int j = 0; j < numofgroups*numofitemsinsecondreduction; j++){
				intragroupbuffer_[i][j] = intergroupbuffer_[j];
			}
		}
	}

	for (unsigned int i = 0; i < numofnodesingroup; i++){
		unsigned int source = groupnumber*numofnodesingroup + i;
		if (rank != source){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			/*for (unsigned int j = 0; j < numofitemsinsecondreduction; j++){
				for(unsigned int k = 0; k < numofgroups; k++){
					allreduceresult[i*numofgroups + j*size + k] = intragroupbuffer_[i][j*numofgroups + k];
				}
			}*/
			unsigned int m = numofitemsinsecondreduction*numofgroups;
			unsigned int a = numofitemsinsecondreduction;
			for (unsigned int j = 0; j < m; j++){
				unsigned int index = (j-j%a + m*(j%a))/a + i*m;
				allreduceresult[index] = intragroupbuffer_[i][j];
			}

		} else { //copy from local buffer -> intergroupbuffer_
			unsigned int m = numofitemsinsecondreduction*numofgroups;
			unsigned int a = numofitemsinsecondreduction;
			for (unsigned int j = 0; j < m; j++){
				unsigned int index = (j-j%a + m*(j%a))/a + i*m;
				allreduceresult[index] = intragroupbuffer_[i][j];
			}
		}
	}
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		unsigned int destination = groupnumber*numofnodesingroup + i;
		if (rank != destination){
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for intra-group allgather\n", MPI_Wtime() - dblasttimer);
	}
#endif

//#define DEBUG4
#if defined(DEBUG4)
	printf("From rank %d|\t, intra group buffer_\n", rank);
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		printf("\t\t");
		for (unsigned int j = 0; j < numofgroups*numofitemsinsecondreduction; j ++){
			printf("\t%.0f", intragroupbuffer_[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for(unsigned int i = 0; i < 100000; i++);
	printf("From rank %d, allreduce result\n\t", rank);
	for(unsigned int i = 0; i < NUM_ITEMS_ROUND; i++){
		printf("\t%.0f", allreduceresult[i]);
	}
	printf("\n");
#endif

	/// Stop timer
	MPI_Barrier(MPI_COMM_WORLD);
	double kimrdtime = MPI_Wtime() - start_time;
	for (unsigned int i = 0; i < numofnodesingroup; i++){
		free(intragroupbuffer_[i]);
	}
	free(intragroupbuffer_);

	free(intergroupbuffer_);
	free(reqrecvs);
	free(reqsends);

	start_time = MPI_Wtime();
	float *allreduceresultlib = (float*)malloc(sizeof(float)*NUM_ITEMS_ROUND);
	for (unsigned int j = 0; j < NUM_ITEMS_ROUND; j++) {
		data[j] = (float)(j);
	}
	MPI_Allreduce(data, allreduceresultlib, NUM_ITEMS_ROUND, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "%s,%.7lf,%.7lf,%d\n", networkshape, kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
	}

	// Compare the result
	/*float sum_1 = 0, sum_2 = 0;
	if (rank == 0){
		for (unsigned int i = 0; i < NUM_ITEMS_ROUND; i++){

			if (allreduceresult[i] != allreduceresultlib[i]){
				//printf("Kim data: %f  lib data %f\n", allreduceresult[i], allreduceresultlib[i]);
				//fprintf(stdout, "Checking result %dx%d. Wrong index:\n\t", numofgroups, numofnodesingroup);
				printf("------------------> ERROR");
				exit (1);
			}
		}

#if defined(DEBUG5)
		printf("Kim allreduce: \n");
		for (unsigned int i = 0; i < NUM_ITEMS_ROUND; i++){
			printf(" %.0f", allreduceresult[i]);

		}
		printf("\nLib Allreduce\n");
		for (unsigned int i = 0; i < NUM_ITEMS_ROUND; i++){
			printf(" %.0f", allreduceresultlib[i]);
		}
		printf("\n");
#endif
	}*/

	free(allreduceresultlib);
	free(allreduceresult);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	   ALLGATHER : END	   ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	MPI_Finalize();
	return 0;
}

