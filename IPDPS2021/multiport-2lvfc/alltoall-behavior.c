/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-12-11 08:43:24
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-01-04 14:23:21
 * @ Description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "config.h"

#define KIM_DATA MPI_INT
#define KIM_data int
#define RAND_SEED 721311

static void program_abort(char* message) {
	fprintf(stderr, "Error: %s", message);
}

int h2r(char* hostname, char* networkshape) { //host name to rank for 2lvfc
	int groupnumber, nodenumber;
	int networkshape_a, networkshape_b;

	sscanf(hostname, "%d_%d", &groupnumber, &nodenumber);
	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	return groupnumber * networkshape_b + nodenumber;
}
int h2r_r(const int groupnumber, const int nodenumber, char* networkshape) { //host name to rank for 2lvfc
	int networkshape_a, networkshape_b;

	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	return groupnumber * networkshape_b + nodenumber;
}

void r2h(int rank, char* networkshape, int* groupnumber, int* nodenumber) { // rank to host name for 2lvfc
	int networkshape_a, networkshape_b;

	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	*groupnumber = rank / networkshape_b;
	*nodenumber = rank % networkshape_b;
	return;
}

void r2h_r(int rank, char* topo, char* hostname) { // rank to host name for 2lvfc
	int numofgroups, numofnodes;
	sscanf(topo, "%dx%d", &numofgroups, &numofnodes);
	sprintf(hostname, "%d_%d", rank / numofnodes, rank % numofnodes);
	return;
}
static inline void swap(KIM_data* arr, int a, int b) {
	KIM_data tmp = arr[a];
	arr[a] = arr[b];
	arr[b] = tmp;
	return;
}


int main(int argc, char* argv[])
{
	int size, rank, NUM_ITEMS;
	char hostname[256];
	char topo[256];//for real MPICH test
	int hostname_len;



	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname, &hostname_len);
	NUM_ITEMS = 2; //default numitems

	// FILE *debugfile;
	// char filename[256];
	// sprintf(filename, "debugfiles/%d", rank);
	// debugfile = fopen(filename, "w");

	// Topology optional argument
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--topo")) {
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%s", topo) != 1)) {
				program_abort("Invalid <topology> argument\n");
			}
			else { // prepare fake hostname for testing on MPICH (without simulation
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
	KIM_data* data = (KIM_data*)SMPI_SHARED_MALLOC(sizeof(KIM_data) * NUM_ITEMS * size);
	if (data == NULL) {
		program_abort("Out of memory!");
	}
	char networkshape[256];
	int networkshape_len;
	if (rank == size - 1) {
		strcpy(networkshape, hostname);
		networkshape_len = hostname_len + 1;
		networkshape[networkshape_len] = '\0';
	}
	MPI_Bcast(&networkshape_len, 1, MPI_INT, size - 1, MPI_COMM_WORLD);
	// Send the shape of the topology to all node
	if (MPI_SUCCESS != MPI_Bcast(networkshape, networkshape_len, MPI_CHAR, size - 1, MPI_COMM_WORLD)) {
		program_abort("Broadcast networkshape fail\n");
	}
#if defined(DEBUG0)
	if (rank == 0) {
		printf("Network shape 1: %s\n", networkshape);
	}
#endif

	// All rank fill the buffer with random data
	srandom(RAND_SEED + rank);
	for (int j = 0; j < NUM_ITEMS * size; j++) {
		data[j] = (KIM_data)(j / NUM_ITEMS + rank * 10);
	}

#if defined(DEBUG1)
	fprintf(debugfile, "Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS * size; i++) {
		fprintf(debugfile, "%d\t", data[i]);
	}
	fprintf(debugfile, "\n");
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < 1000000; i++);
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

	MPI_Request* reqrecvs, * reqsends;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLTOALL : START	  ///////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Step 1: Intra group alltoall  ////////////////////////////////////////////////////////////////
	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * (numofnodesingroup));
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * (numofnodesingroup));

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	double dblasttimer = MPI_Wtime();
#endif


	// Receive data (KIM_data**)malloc(sizeof(KIM_data*)*numofnodesingroup);
	// KIM_data** recvbufintra = (KIM_data**)malloc(sizeof(KIM_data*) * numofnodesingroup);
	// for (int i = 0; i < numofnodesingroup; i++) {
	// 	if (i != nodenumber) {
	// 		recvbufintra[i] = (KIM_data*)malloc(sizeof(KIM_data) * (numofgroups + 5) * NUM_ITEMS);
	// 		for (int k = 0; k < numofgroups * NUM_ITEMS; k++) {
	// 			recvbufintra[i][k] = -1;
	// 		}
	// 	}
	// }
	KIM_data* recvbufintra = (KIM_data*)SMPI_SHARED_MALLOC(sizeof(KIM_data) * (numofgroups + 5) * NUM_ITEMS);
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			int source = h2r_r(groupnumber, i, networkshape);
			MPI_Irecv(recvbufintra, numofgroups * NUM_ITEMS, KIM_DATA, source, source, MPI_COMM_WORLD, &(reqrecvs[i]));
		}
	}

	// Prepare data for intra group transfer
	// KIM_data** sendbufintra = (KIM_data**)malloc(sizeof(KIM_data*) * numofnodesingroup);
	// for (int i = 0; i < numofnodesingroup; i++) {
	// 	if (i != nodenumber) {
	// 		sendbufintra[i] = (KIM_data*)malloc(sizeof(KIM_data) * numofgroups * NUM_ITEMS);
	// 	}
	// }

	KIM_data* sendbufintra = (KIM_data*)SMPI_SHARED_MALLOC(sizeof(KIM_data) * numofgroups * NUM_ITEMS);

	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			// fprintf(debugfile, "Send data from %d_%d to %d_%d: ", groupnumber, nodenumber, groupnumber, i);
			for (int j = 0; j < numofgroups; j++) {
				// fprintf(debugfile, "%d_%d ", j, i); // Index of data
				// prepare sendbuf
				//memcpy(&sendbufintra[NUM_ITEMS*j], )
				int index = h2r_r(j, i, networkshape);
				// for (int k = 0; k < NUM_ITEMS; k++){
				// 	sendbufintra[i][j*NUM_ITEMS + k] = data[index*NUM_ITEMS + k];
				//     // fprintf(debugfile, "%d ", sendbufintra[i][j*NUM_ITEMS + k]);
				// }
				memcpy(&sendbufintra[j * NUM_ITEMS], &data[index * NUM_ITEMS], NUM_ITEMS * sizeof(KIM_data));
			}
			int dest = h2r_r(groupnumber, i, networkshape);
			MPI_Isend(sendbufintra, NUM_ITEMS * numofgroups, KIM_DATA, dest, rank, MPI_COMM_WORLD, &(reqsends[i]));
			// fprintf(debugfile, "\n");
		}
	}

	KIM_data* alltoallresult = data;//(KIM_data*)SMPI_SHARED_MALLOC(sizeof(KIM_data) * NUM_ITEMS * size);
	for (int i = 0; i < NUM_ITEMS * size; i++) {
		alltoallresult[i] = -1;
	}

	// Copy from data to result
	for (int i = 0; i < NUM_ITEMS; i++) {
		alltoallresult[rank * NUM_ITEMS + i] = data[rank * NUM_ITEMS + i];
	}

	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			// fprintf(debugfile, "Node %d_%d receive data from %d_%d: ", groupnumber, nodenumber, groupnumber, i);
			// for (int j = 0; j < numofgroups; j++){
			// 	fprintf(debugfile, "%d_%d ", j, nodenumber);
			// }
			// fprintf(debugfile, "\n");
		}
	}
	// fprintf(debugfile, "Status error code: ");
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			MPI_Wait(&(reqsends[i]), MPI_STATUS_IGNORE);
			// fprintf(debugfile, "%d ", (statussends[i]).MPI_SOURCE);
			// fprintf(debugfile, "%ld ", statusrecvs[i].count);

			// free(sendbufintra[i]);
		}
	}
	// fprintf(debgit commit -m ".gitignore is now working"ugfile, "\n");
	// SMPI_SHARED_FREE(sendbufintra);
#if defined(DEBUG1)
	fprintf(debugfile, "From rank: %d|\t recvbufintra\n", rank);
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			for (int j = 0; j < numofgroups * NUM_ITEMS; j++) {
				fprintf(debugfile, "\t\t%d", recvbufintra[i][j]);
			}
			fprintf(debugfile, "\n");
		}
	}
	fprintf(debugfile, "\n");
#endif

	free(reqsends);
	free(reqrecvs);

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		fprintf(stdout, "%.7lf\tTime for inter-group all-gather\n", MPI_Wtime() - dblasttimer);
	}
#endif
#if !defined(COMPARE_BUILDIN)
	// free(data);
#endif

	// Step 2: Intra group alltoall  ////////////////////////////////////////////////////////////////
	//Final result


	MPI_Request** reqrecvss = (MPI_Request**)malloc(sizeof(MPI_Request*) * (numofnodesingroup));
	MPI_Request** reqsendss = (MPI_Request**)malloc(sizeof(MPI_Request*) * (numofnodesingroup));
	for (int i = 0; i < numofnodesingroup; i++) {
		reqrecvss[i] = (MPI_Request*)malloc(sizeof(MPI_Request) * numofgroups);
		reqsendss[i] = (MPI_Request*)malloc(sizeof(MPI_Request) * numofgroups);
	}

#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	dblasttimer = MPI_Wtime();
#endif

	KIM_data* sendbufinter = (KIM_data*)SMPI_SHARED_MALLOC(sizeof(KIM_data) * NUM_ITEMS);


	for (int i = 0; i < numofgroups; i++) {
		if (i != groupnumber) {
			int source = h2r_r(i, nodenumber, networkshape);
			int dataindex = source;
			MPI_Irecv(&alltoallresult[dataindex * NUM_ITEMS], NUM_ITEMS, KIM_DATA, source, 0, MPI_COMM_WORLD, &(reqrecvss[nodenumber][i]));
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		if (i != groupnumber) {
			int destination = h2r_r(i, nodenumber, networkshape);
			int dataindex = destination;
			MPI_Isend(&data[dataindex * NUM_ITEMS], NUM_ITEMS, KIM_DATA, destination, 0, MPI_COMM_WORLD, &reqsendss[nodenumber][i]);
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		if (i != groupnumber) {
			MPI_Wait(&(reqrecvss[nodenumber][i]), MPI_STATUS_IGNORE);
		}
	}



	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			for (int j = 0; j < numofgroups; j++) {
				if (j != groupnumber) {
					int source = h2r_r(j, nodenumber, networkshape);
					int dataindex = h2r_r(j, i, networkshape);
					// printf("From node %d wait to receive data %d from node %d\n", rank, dataindex, source);
					MPI_Irecv(&alltoallresult[dataindex * NUM_ITEMS], NUM_ITEMS, KIM_DATA, source,
						dataindex, MPI_COMM_WORLD, &reqrecvss[i][j]);
				}
			}
		}
	}

	// for k in intra group buffer (numofnodesingroup)
		// for i in numofgroup 
		// if i == groupnumber (data of this group)
		//     save to final result
		// else 
		//     send to the right node in the inter node group
		//     node i_nodenumber
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			for (int j = 0; j < numofgroups; j++) {
				if (j == groupnumber) {
					// save data (data from intra group) to final result
					int dataindex = h2r_r(groupnumber, i, networkshape);
					// for (int k = 0; k < NUM_ITEMS; k++){
					// 	alltoallresult[dataindex*NUM_ITEMS + k] = recvbufintra[i][j*NUM_ITEMS + k];
					// }
					memcpy(&alltoallresult[dataindex * NUM_ITEMS], &recvbufintra[j * NUM_ITEMS], NUM_ITEMS * sizeof(KIM_data));
				}
				else {
					// send data to the right node
					int dataindex = h2r_r(groupnumber, i, networkshape);
					int destination = h2r_r(j, nodenumber, networkshape);
					// printf("From node %d send data %d to node %d\n", rank, dataindex, destination);
					MPI_Isend(&recvbufintra[j * NUM_ITEMS], NUM_ITEMS, KIM_DATA, destination, dataindex, MPI_COMM_WORLD, &reqsendss[i][j]);
				}
			}
		}
	}



	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			for (int j = 0; j < numofgroups; j++) {
				if (j != groupnumber) {
					MPI_Wait(&reqrecvss[i][j], MPI_STATUS_IGNORE);
				}
			}
		}
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			for (int j = 0; j < numofgroups; j++) {
				if (j != groupnumber) {
					MPI_Wait(&reqsendss[i][j], MPI_STATUS_IGNORE);
				}
			}
		}
	}

	for (int i = 0; i < numofnodesingroup; i++) {
		free(reqrecvss[i]);
		free(reqsendss[i]);
	}
	free(reqrecvss);
	free(reqsendss);
	// SMPI_SHARED_FREE(sendbufinter);

#if defined(DEBUG4)
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < 100000; i++);
	if (rank == 0) {
		printf("From rank %d, allgather buffer\n\t", rank);
		for (int i = 0; i < NUM_ITEMS * size; i++) {
			printf("\t%d", allgatherresult[i]);
		}
	}

	printf("\n");
#endif



#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		fprintf(stdout, "%.7lf\tTime for intra-group allgather\n", MPI_Wtime() - dblasttimer);
	}
#endif
#if defined(DEBUG4)
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < 100000; i++);
	fprintf(debugfile, "From rank %d, allgather result\n\t", rank);
	for (int i = 0; i < NUM_ITEMS * size; i++) {
		fprintf(debugfile, "\t%d", allgatherresult[i]);
	}
	fprintf(debugfile, "\n");
#endif

	/// Stop timer
	MPI_Barrier(MPI_COMM_WORLD);
	double kimrdtime = MPI_Wtime() - start_time;


	if ((0 == rank)) {
		fprintf(stdout, "%dx%d,%.7lf,%d\n", numofgroups, numofnodesingroup, kimrdtime, NUM_ITEMS);
	}

#if defined(COMPARE_BUILDIN)
	start_time = MPI_Wtime();
	KIM_data* alltoallresultlib = (KIM_data*)malloc(sizeof(KIM_data) * NUM_ITEMS * size);
	MPI_Alltoall(data, NUM_ITEMS, KIM_DATA, alltoallresultlib, NUM_ITEMS, KIM_DATA, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "%dx%d,%.7lf,%.7lf,%d\n", numofgroups, numofnodesingroup, kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
	}

	// Compare the result
	if (rank == 0) {
		for (int i = 0; i < NUM_ITEMS * size; i++) {
			if (alltoallresult[i] != alltoallresultlib[i]) {
				fprintf(stdout, "%s, %s\n", networkshape, "Alltoall wrong");
				break;
			}
		}
		free(data);
#endif

#if defined(DEBUG5)
		printf("Kim alltoall: \n");
		for (int i = 0; i < NUM_ITEMS * size; i++) {
			printf(" %d", alltoallresult[i]);
		}
		printf("\nLib Alltoall\n");
		for (int i = 0; i < NUM_ITEMS * size; i++) {
			printf(" %d", alltoallresultlib[i]);
		}
		printf("\n");
#endif

#if defined(COMPARE_BUILDIN)
	}
	free(alltoallresultlib);
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	   ALLTOALL : END	   //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	// fclose(debugfile);
	MPI_Finalize();
	return 0;
}
