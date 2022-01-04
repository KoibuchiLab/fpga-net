/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-09-25 23:13:38
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-01-04 18:35:39
 * @ Description:
 */


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "config.h"
#include <assert.h>


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
	char hostname[256];
	char topo[256];//for real MPICH test
	int hostname_len;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname,&hostname_len);
	NUM_ITEMS = 500; //default numitems
	FILE *debug = stdout;
	// char filename[256];
	// sprintf(filename, "log/%d", rank);
	// debug = fopen(filename, "w");
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
	int NUM_ITEMS_ROUND = NUM_ITEMS - NUM_ITEMS%size;
	float* data = (float*)SMPI_SHARED_MALLOC(sizeof(float) * NUM_ITEMS_ROUND);
	
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
		fprintf(debug, "Network shape 1: %s\n", networkshape);
	}
#endif
	
	

#if defined(DEBUG1)
	fprintf(debug, "Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS_ROUND; i++){
		fprintf(debug, "%.0f\t", data[i]);
	}
	fprintf(debug, "\n");
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

	if (rank != h2r(hostname, networkshape))
		program_abort("Computed rank must equal to real rank\n");
	int numofgroups, numofnodesingroup;
	sscanf(networkshape, "%d_%d", &numofgroups, &numofnodesingroup);
	numofgroups++;
	numofnodesingroup++;

	// All rank fill the buffer with random data
	srandom(RAND_SEED + rank);
	for (int j = 0; j < NUM_ITEMS_ROUND; j++) {
		data[j] = (float)(1 + 1.0 * (random() % 9));// j% numofnodesingroup + groupnumber;//
	}

#if defined(DEBUG0)
	fprintf(debug, "Host name %s \t| rank %d \t| rank computed %d\t| %d_%d\n", hostname, rank, h2r(hostname, networkshape), \
			groupnumber, nodenumber);
	if(rank == 0){
		fprintf(debug, "Network shape: %s\n", networkshape);
	}
#endif
	
	//Step 1: Intra group exchange  /////////////////////////////////////////////////////////////////
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	double dblasttimer = MPI_Wtime();
#endif
	int numofitemsineachchunk = NUM_ITEMS_ROUND / numofnodesingroup;

	//Prepare buffer to receive data
	float* intragroupbuffer0 = (float*)SMPI_SHARED_MALLOC(sizeof(float) * numofitemsineachchunk);
	// for (int i = 0; i < numofnodesingroup; i++){
	// 	intragroupbuffer[i] = (float*)malloc(sizeof(float)*numofitemsineachchunk);
	// 	for(int j = 0; j < numofitemsineachchunk; j++){
	// 		intragroupbuffer[i][j] = 0;
	// 	}
	// }

	MPI_Request * reqsends = (MPI_Request*) malloc(sizeof(MPI_Request)*numofnodesingroup);
	MPI_Request * reqrecvs = (MPI_Request*) malloc(sizeof(MPI_Request)*numofnodesingroup);


	// Place MPI_Irecv
	for (int i = 0; i < numofnodesingroup; i++){
		// Compute source
		int source = numofnodesingroup*groupnumber + i;
		if (source != rank){
			MPI_Irecv(intragroupbuffer0, numofitemsineachchunk, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
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
				intragroupbuffer0[j] = data[i*numofitemsineachchunk + j];
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
	fprintf(debug, "Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS_ROUND; i++){
		fprintf(debug, "%.0f\t", data[i]);
	}
	fprintf(debug, "\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i <1000000; i++);
	fprintf(debug, "Intra group Reduce scatter from rank %d: \n", rank);
	for (int i = 0; i < numofnodesingroup; i++){
		fprintf(debug, "\t\t");
		for (int j = 0; j < numofitemsineachchunk; j++){
			fprintf(debug, "%.0f\t", intragroupbuffer[i][j]);
		}
		fprintf(debug, "\n");
	}
	fprintf(debug, "\n");
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
	// Could reduce time by pipeline with receiveing data
	float* intragroupreductionresult = (float*)SMPI_SHARED_MALLOC(sizeof(float) * numofitemsineachchunk);
	for (int i = 0; i < numofitemsineachchunk; i++){
		intragroupreductionresult[i] = 0;
	}
	for (int i = 0; i < numofnodesingroup; i++){
		for (int j = 0; j < numofitemsineachchunk; j++){
			intragroupreductionresult[j] = reduce(intragroupreductionresult[j], intragroupbuffer0[j], "sum");
		}
	}



#if defined(DEBUG2)
	fprintf(debug, "From rank %d | Intra group reduction result:\t", rank);
	for (int i = 0; i < numofitemsineachchunk; i++){
		fprintf(debug, "%.0f\t", intragroupreductionresult[i]);
	}
	fprintf(debug, "\n");
#endif
#if defined(DEBUG1)
	fprintf(debug, "Intra group reduction result:\t");
	for (int i = 0; i < numofitemsineachchunk; i++){
		fprintf(debug, "%.0f\t", intragroupreductionresult[i]);
	}
	fprintf(debug, "\n");
#endif

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for intra-group reduce scatter computation time\n", MPI_Wtime() - dblasttimer);
	}
#endif
	free(reqsends);
	free(reqrecvs);


	//Step 2: Inter group exchange  /////////////////////////////////////////////////////////////////
	//Prepare buffer to receive data
	// int numofitemsinsecondreduction = NUM_ITEMS_ROUND / size;
	int numofitemsinsecondreduction = NUM_ITEMS_ROUND / size;
	
	// float **intergroupbuffer = (float**)malloc(sizeof(float*)*numofgroups);
	//intergroupbuffer[i] = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);
	float* intergroupbuffer0 = (float*)malloc(sizeof(float) * numofitemsinsecondreduction);
	// for (int i = 0; i < numofgroups; i++){
	// 	intergroupbuffer[i] = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);
	// 	for (int j = 0; j < numofitemsinsecondreduction; j++){
	// 		intergroupbuffer[i][j] = 0; // should carefull for other reduce operation
	// 	}
	// }
	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif
	// Receive data
	for (int i = 0; i < numofgroups; i++){
		int source = i*numofnodesingroup + nodenumber;
		MPI_Irecv(intergroupbuffer0, numofitemsinsecondreduction, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
	}


	// Send data
	float **sendbuf00 = (float**)malloc(sizeof(float*)*numofgroups);
	for (int i = 0; i < numofgroups; i++){
		sendbuf00[i] = (float*)malloc(sizeof(float) * numofitemsinsecondreduction);
	}

	for (int i = 0; i < numofgroups; i++){
		int destination = i * numofnodesingroup + nodenumber;

		//prepare data for sendbuf
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			//read data from intragroupreductionresult
			sendbuf00[i][j] = intragroupreductionresult[j*numofgroups + i];
		}
		if (destination != rank){
			MPI_Isend(sendbuf00[i], numofitemsinsecondreduction, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);
		} else { //copy from buffer
			for (int j = 0; j < numofitemsinsecondreduction; j++){
				intergroupbuffer0[j] = sendbuf00[i][j];
			}
		}
	}

	for (int i = 0; i < numofgroups; i++){
		int source = i * numofnodesingroup + nodenumber;
		if(rank != source){
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	for (int i = 0; i < numofgroups; i++){
		int destination = i * numofnodesingroup + nodenumber;
		if (rank != destination){
			MPI_Wait(&reqsends[i], MPI_STATUSES_IGNORE);
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		free(sendbuf00[i]);
	}
	free(sendbuf00);
	free(reqsends);
	free(reqrecvs);
#if defined(DEBUG2) 
	fprintf(debug, "From rank %d | Inter group buffer: ", rank);
	for (int i = 0; i < numofgroups; i++){
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			fprintf(debug, "\t%.0f", intergroupbuffer[i][j]);
		}
	}
	fprintf(debug, "\n");
#endif

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for inter-group reduce scatter communication time\n", MPI_Wtime() - dblasttimer);
	}
#endif

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif
	//Execute reduction -> change += to reduction function
	// float* intergroupreductionresult = (float*)malloc(sizeof(float) * numofitemsinsecondreduction);
	float *intergroupreductionresult = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);
	for (int i = 0; i < numofitemsinsecondreduction; i++){
		intergroupreductionresult[i] = 0;
	}
	for (int i = 0; i < numofgroups; i++){
		for (int j = 0; j < numofitemsinsecondreduction; j++){
			intergroupreductionresult[j] += intergroupbuffer0[j];
		}
	}
	//  tobe delete
	//   check the correctnes with this data
	//   for (int j = 0; j < NUM_ITEMS_ROUND; j++) {
	// 	data[j] = j % numofnodesingroup + groupnumber;//(float)(1 + 1.0 * (random() % 9));
	// }
	// assert(numofitemsinsecondreduction == NUM_ITEMS_ROUND / size);
	// float *testintergroupreduction = (float*)malloc(sizeof(float)*numofnodesingroup);
	// for (int i = 0; i < numofnodesingroup; i++) {
	// 	testintergroupreduction[i] = (21 + 7*i)*3;
	// }
	// if (rank == 0){
	// 	for (int i = 0; i < numofitemsinsecondreduction; i++) {
	// 		int comparevalue = intergroupreductionresult[i] - testintergroupreduction[(i % numofnodesingroup) % numofnodesingroup];
	// 		if (comparevalue != 0) {
	// 			printf("From rank %d Intergroupreductionresult wrong %.0f %.0f\n", rank, intergroupreductionresult[i], \
	// 				testintergroupreduction[(i % numofnodesingroup) % numofnodesingroup]);
	// 		}
	// 	}
	// }
	

	
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for inter-group reduce scatter computation time\n", MPI_Wtime() - dblasttimer);
	}
#endif

#if defined(DEBUG2)
	fprintf(debug, "From rank %d, inter group reduction result: ", rank);
	for (int i = 0; i < numofitemsinsecondreduction; i++){
		fprintf(debug, "\t%.0f", intergroupreductionresult[i]);
	}
	fprintf(debug, "\n");
#endif

	//free(intergroupbuffer);
	free(intragroupreductionresult);
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
	// float* intergroupbuffer_ = (float*)malloc(sizeof(float) * numofgroups * numofitemsinsecondreduction);
	float* intergroupbuffer_0 = (float*)SMPI_SHARED_MALLOC(sizeof(float) * numofgroups * numofitemsinsecondreduction);
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif
	for (int i = 0; i < numofgroups; i++){
		// Compute source
		int source = numofnodesingroup*i + nodenumber;
		if (source != rank){
			MPI_Irecv(&intergroupbuffer_0[i*numofitemsinsecondreduction], numofitemsinsecondreduction, \
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
				intergroupbuffer_0[i*numofitemsinsecondreduction + j] = intergroupreductionresult[j];
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
	fprintf(debug, "From rank: %d|\t intergroubuffer_ (intergroup buffer for allgather)\n\t\t", rank);
	for(int i = 0; i < numofgroups*numofitemsinsecondreduction; i++){
		fprintf(debug, "\t%.0f", intergroupbuffer_[i]);
	}
	fprintf(debug, "\n");
#endif
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for inter-group all-gather\n", MPI_Wtime() - dblasttimer);
	}
#endif

	free(intergroupreductionresult);
	// Step 2: Intra group gather  //////////////////////////////////////////////////////////////////
	//Final result
	float *allreduceresult = data;

	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofnodesingroup);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofnodesingroup);
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif
	
	for (int i = 0; i < NUM_ITEMS_ROUND; i++){
		allreduceresult[i] = 0;
	}
	//Allocate buffer for intra group
	// float* bufreorder = (float*)malloc(sizeof(float) * numofgroups * numofitemsinsecondreduction);
	float* bufreorder = (float*)SMPI_SHARED_MALLOC(sizeof(float) * numofgroups * numofitemsinsecondreduction);
	for (int i = 0; i < numofgroups * numofitemsinsecondreduction; i++) {
		int index = i / numofitemsinsecondreduction + numofgroups * (i % numofitemsinsecondreduction);
		// fprintf(stdout, "%d ", index);
		bufreorder[index] = intergroupbuffer_0[i];
	}
	// fprintf(stdout, "\n");
	
	
	for (int i = 0; i < numofnodesingroup; i++){
		int source = groupnumber*numofnodesingroup + i;
		int m = numofitemsinsecondreduction * numofgroups;
		if (rank != source){
			MPI_Irecv(&allreduceresult[m * i], numofgroups * numofitemsinsecondreduction, MPI_FLOAT, \
				source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (int i = 0; i < numofnodesingroup; i++){
		int destination = groupnumber*numofnodesingroup + i;
		if(rank != destination){
			MPI_Isend(bufreorder, numofgroups* numofitemsinsecondreduction, MPI_FLOAT, \
				destination, 1, MPI_COMM_WORLD, &reqsends[i]);
		} else {
			int m = numofitemsinsecondreduction * numofgroups;
			memcpy(&allreduceresult[m * i], bufreorder, sizeof(float)* m);
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
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		fprintf(stdout, "%.7lf\tTime for intra-group allgather\n", MPI_Wtime() - dblasttimer);
	}
#endif
#if defined(DEBUG4)
	fprintf(debug, "From rank %d|\t, intra group buffer_\n", rank);
	for (int i = 0; i < numofnodesingroup; i++){
		fprintf(debug, "\t\t");
		for (int j = 0; j < numofgroups*numofitemsinsecondreduction; j ++){
			fprintf(debug, "\t%.0f", intragroupbuffer_[i][j]);
		}
		fprintf(debug, "\n");
	}
	fprintf(debug, "\n");

	fprintf(debug, "From rank %d, allreduce result\n\t", rank);
	for(int i = 0; i < NUM_ITEMS_ROUND; i++){
		fprintf(debug, "\t%.0f", allreduceresult[i]);
	}
	fprintf(debug, "\n");
#endif

	/// Stop timer
	MPI_Barrier(MPI_COMM_WORLD);
	double kimrdtime = MPI_Wtime() - start_time;
	
	free(bufreorder);
	free(reqrecvs);
	free(reqsends);
	start_time = MPI_Wtime();
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "%dx%d,%.7lf,%d\n", numofgroups, numofnodesingroup, kimrdtime, NUM_ITEMS);
	}


#if defined(DEBUG5)
		fprintf(debug, "Kim allreduce: \n");
		for (int i = 0; i < NUM_ITEMS_ROUND; i++){
			fprintf(debug, " %.0f", allreduceresult[i]);
		}
		fprintf(debug, "\nLib Allreduce\n");
		for (int i = 0; i < NUM_ITEMS_ROUND; i++){
			fprintf(debug, " %.0f", allreduceresultlib[i]);
		}
		fprintf(debug, "\n");
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	   ALLGATHER : END	   ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// fclose(debug);
	
	MPI_Finalize();
	return 0;
}
