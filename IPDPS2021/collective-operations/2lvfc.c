/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-12-19 17:37:49
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-01-08 22:35:09
 * @ Description:
 */
#include"kmpi.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void program_abort(char* message);

int h2r(char* hostname, char* networkshape);
int h2r_r(const int groupnumber, const int nodenumber, char* networkshape);
void r2h(int rank, char* networkshape, int* groupnumber, int* nodenumber);
void r2h_r(int rank, char* topo, char* hostname);
static inline void swap(float* arr, int a, int b);

int KMPI_Allgatherf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
	float* recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm){
	int size, rank;
	int groupnumber, nodenumber; // Hostname define in xml is groupnumber_nodenumber
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	
	char hostname[256];
	char topo[256];//for real MPICH test
	int hostname_len;
	MPI_Get_processor_name(hostname, &hostname_len);

	char networkshape[256];
	int networkshape_len;
	if (rank == size - 1) {
		strcpy(networkshape, hostname);
		networkshape_len = hostname_len + 1;
		networkshape[networkshape_len] = '\0';
	}
	MPI_Bcast(&networkshape_len, 1, MPI_INT, size - 1, comm);
	// Send the shape of the topology to all node
	if (MPI_SUCCESS != MPI_Bcast(networkshape, networkshape_len, MPI_CHAR, size - 1, comm)) {
		program_abort("Broadcast networkshape fail\n");
	}

	r2h(rank, networkshape, &groupnumber, &nodenumber);
// #define DEBUG0
#if defined(DEBUG0)
	printf("Rank: %d | Host name: %s | Length: %d\n", rank, hostname, hostname_len);
#endif

	if (rank != h2r(hostname, networkshape))
		program_abort("Computed rank must equal to real rank\n");
	int numofgroups, numofnodesingroup;
	sscanf(networkshape, "%d_%d", &numofgroups, &numofnodesingroup);
	numofgroups++;
	numofnodesingroup++;

	MPI_Request* reqrecvs, * reqsends;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLGATHER : START	  ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Step 1: Inter group gather  //////////////////////////////////////////////////////////////////
	// send inter group reduction result to other groups
	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * (numofgroups + 1));
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * (numofgroups + 1));
	float* intergroupbuffer_ = (float*)malloc(sizeof(float) * numofgroups * sendcount);
	
#if defined(TIME_FOR_EACH_STEP)
	MPI_Barrier(comm);
	double dblasttimer = MPI_Wtime();
#endif
	for (int i = 0; i < numofgroups; i++) {
		// Compute source
		int source = numofnodesingroup * i + nodenumber;
		if (source != rank) {
			MPI_Irecv(&intergroupbuffer_[i * recvcount], recvcount, \
				MPI_FLOAT, source, 1, comm, &reqrecvs[i]);
		}
		int destination = numofnodesingroup * i + nodenumber;
		if (destination != rank) {
			MPI_Isend(sendbuf, sendcount, MPI_FLOAT, destination, 1, \
				comm, &reqsends[i]);
		}
		else {
			//intergroupbuffer_[i] <- data (data is on this process so dont need to send)
			for (int j = 0; j < sendcount; j++) {
				intergroupbuffer_[i * sendcount + j] = sendbuf[j];
			}
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		// Compute source
		int source = numofnodesingroup * i + nodenumber;
		if (source != rank) {
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		// Compute destination
		int destination = numofnodesingroup * i + nodenumber;
		if (destination != rank) {
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}

	free(reqsends);
	free(reqrecvs);
	// Step 2: Intra group gather  //////////////////////////////////////////////////////////////////
	//Final result
	// float* allgatherresult = (float*)malloc(sizeof(float) * sendcount * size);

	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * (numofnodesingroup + 1));
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * (numofnodesingroup + 1));

	for (int i = 0; i < numofnodesingroup; i++) {
		int source = groupnumber * numofnodesingroup + i;
		if (rank != source) {
			MPI_Irecv(&recvbuf[i * numofgroups * recvcount], numofgroups * recvcount, MPI_FLOAT, \
				source, 1, comm, &reqrecvs[i]);
		}

		int destination = groupnumber * numofnodesingroup + i;
		if (rank != destination) {
			MPI_Isend(intergroupbuffer_, numofgroups * sendcount, MPI_FLOAT, \
				destination, 1, comm, &reqsends[i]);
		}
		else {
			for (int j = 0; j < numofgroups * sendcount; j++) {
				recvbuf[i * numofgroups * sendcount + j] = intergroupbuffer_[j];
			}
		}
	}

	for (int i = 0; i < numofnodesingroup; i++) {
		int source = groupnumber * numofnodesingroup + i;
		if (rank != source) {
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		int destination = groupnumber * numofnodesingroup + i;
		if (rank != destination) {
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}

	// Allocate buffer for intra group
	// Size = [numofnodesingroup][numofgroup*numofitemkim
	float** intragroupbuffer_ = (float**)malloc(sizeof(float*) * numofnodesingroup);
	for (int i = 0; i < numofnodesingroup; i++) {
		intragroupbuffer_[i] = (float*)malloc(sizeof(float) * numofgroups * sendcount);
		for (int j = 0; j < numofgroups * sendcount; j++) {
			intragroupbuffer_[i][j] = 0; // Should careful for other gather op
		}
	}

	for (int i = 0; i < numofnodesingroup; i++) {
		int source = groupnumber * numofnodesingroup + i;
		if (rank != source) {
			MPI_Irecv(intragroupbuffer_[i], numofgroups * recvcount, MPI_FLOAT, \
				source, 1, comm, &reqrecvs[i]);
		}
		int destination = groupnumber * numofnodesingroup + i;
		if (rank != destination) {
			MPI_Isend(intergroupbuffer_, numofgroups * sendcount, MPI_FLOAT, \
				destination, 1, comm, &reqsends[i]);
		}
		else {
			for (int j = 0; j < numofgroups * sendcount; j++) {
				intragroupbuffer_[i][j] = intergroupbuffer_[j];
			}
		}
	}

	for (int i = 0; i < numofnodesingroup; i++) {
		int source = groupnumber * numofnodesingroup + i;
		if (rank != source) {
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
		for (int j = 0; j < sendcount * numofgroups; j++) {
			int index = j % sendcount + i * sendcount + (j / sendcount) * numofnodesingroup * sendcount;
			recvbuf[index] = intragroupbuffer_[i][j];
		}
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		int destination = groupnumber * numofnodesingroup + i;
		if (rank != destination) {
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}
	free(intergroupbuffer_);
	
	for (int i = 0; i < numofnodesingroup; i++) {
		free(intragroupbuffer_[i]);
	}
	free(intragroupbuffer_);
	free(reqrecvs);
	free(reqsends);
	return MPI_SUCCESS;
}

#define KIM_DATA MPI_FLOAT
#define KIM_data float

int KMPI_Alltoallf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
	float* recvbuf, int recvcount, MPI_Datatype recvtype,
	MPI_Comm comm){
	int size, rank;
	char hostname[256];
	char topo[256];//for real MPICH test
	int hostname_len;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname, &hostname_len);


	// FILE *debugfile;
	// char filename[256];
	// sprintf(filename, "debugfiles/%d", rank);
	// debugfile = fopen(filename, "w");

	
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
	KIM_data** recvbufintra = (KIM_data**)malloc(sizeof(KIM_data*) * numofnodesingroup);
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			recvbufintra[i] = (KIM_data*)malloc(sizeof(KIM_data) * (numofgroups + 5) * recvcount);
			for (int k = 0; k < numofgroups * recvcount; k++) {
				recvbufintra[i][k] = -1;
			}
		}
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			int source = h2r_r(groupnumber, i, networkshape);
			MPI_Irecv(recvbufintra[i], numofgroups * recvcount, KIM_DATA, source, source, MPI_COMM_WORLD, &(reqrecvs[i]));
		}
	}

	// Prepare data for intra group transfer
	KIM_data** sendbufintra = (KIM_data**)malloc(sizeof(KIM_data*) * numofnodesingroup);
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			sendbufintra[i] = (KIM_data*)malloc(sizeof(KIM_data) * numofgroups * sendcount);
		}
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			// fprintf(debugfile, "Send data from %d_%d to %d_%d: ", groupnumber, nodenumber, groupnumber, i);
			for (int j = 0; j < numofgroups; j++) {
				// fprintf(debugfile, "%d_%d ", j, i); // Index of data
				// prepare sendbuf
				//memcpy(&sendbufintra[sendcount*j], )
				int index = h2r_r(j, i, networkshape);
				// for (int k = 0; k < sendcount; k++){
				// 	sendbufintra[i][j*sendcount + k] = data[index*sendcount + k];
				//     // fprintf(debugfile, "%d ", sendbufintra[i][j*sendcount + k]);
				// }
				memcpy(&sendbufintra[i][j * sendcount], &sendbuf[index * sendcount], sendcount * sizeof(KIM_data));
			}
			int dest = h2r_r(groupnumber, i, networkshape);
			MPI_Isend(sendbufintra[i], sendcount * numofgroups, KIM_DATA, dest, rank, MPI_COMM_WORLD, &(reqsends[i]));
			// fprintf(debugfile, "\n");
		}
	}

	// Copy from data to result
	for (int i = 0; i < sendcount; i++) {
		recvbuf[rank * sendcount + i] = sendbuf[rank * sendcount + i];
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

			free(sendbufintra[i]);
		}
	}
	// fprintf(debgit commit -m ".gitignore is now working"ugfile, "\n");
	free(sendbufintra);
#if defined(DEBUG1)
	fprintf(debugfile, "From rank: %d|\t recvbufintra\n", rank);
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			for (int j = 0; j < numofgroups * sendcount; j++) {
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

	KIM_data* sendbufinter = (KIM_data*)malloc(sizeof(KIM_data) * sendcount);


	for (int i = 0; i < numofgroups; i++) {
		if (i != groupnumber) {
			int source = h2r_r(i, nodenumber, networkshape);
			int dataindex = source;
			MPI_Irecv(&recvbuf[dataindex * recvcount], recvcount, KIM_DATA, source, 0, MPI_COMM_WORLD, &(reqrecvss[nodenumber][i]));
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		if (i != groupnumber) {
			int destination = h2r_r(i, nodenumber, networkshape);
			int dataindex = destination;
			MPI_Isend(&sendbuf[dataindex * sendcount], sendcount, KIM_DATA, destination, 0, MPI_COMM_WORLD, &reqsendss[nodenumber][i]);
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
					MPI_Irecv(&recvbuf[dataindex * recvcount], recvcount, KIM_DATA, source,
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
					// for (int k = 0; k < recvcount; k++){
					// 	alltoallresult[dataindex*recvcount + k] = recvbufintra[i][j*recvcount + k];
					// }
					memcpy(&recvbuf[dataindex * recvcount], &recvbufintra[i][j * recvcount], recvcount * sizeof(KIM_data));
				}
				else {
					// send data to the right node
					int dataindex = h2r_r(groupnumber, i, networkshape);
					int destination = h2r_r(j, nodenumber, networkshape);
					// printf("From node %d send data %d to node %d\n", rank, dataindex, destination);
					MPI_Isend(&recvbufintra[i][j * sendcount], sendcount, KIM_DATA, destination, dataindex, MPI_COMM_WORLD, &reqsendss[i][j]);
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
	free(sendbufinter);
}

int KMPI_Init(int rank, int size){
	return MPI_SUCCESS;
}

//Behavior
int KMPI_Allreducef(const float* sendbuf, float* recvbuf, int count,
	MPI_Datatype datatype, MPI_Op op, MPI_Comm comm){
	int size, rank;
	char hostname[256];
	char topo[256];//for real MPICH test
	int hostname_len;

	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &size);
	MPI_Get_processor_name(hostname, &hostname_len);
	

	//	if (rank == 0) printf("Chunksize: %d\n", chunksize);
	size_t NUM_ITEMS_ROUND = ((count - 1) / size + 1)* size;
	if (NUM_ITEMS_ROUND != count) return MPI_ERR_ASSERT;

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

	//Step 1: Intra group exchange  /////////////////////////////////////////////////////////////////
	int numofitemsineachchunk = NUM_ITEMS_ROUND / numofnodesingroup;

	//Prepare buffer to receive data
	float** intragroupbuffer = (float**)malloc(sizeof(float*) * numofnodesingroup);
	for (int i = 0; i < numofnodesingroup; i++) {
		intragroupbuffer[i] = (float*)malloc(sizeof(float) * numofitemsineachchunk);
		for (int j = 0; j < numofitemsineachchunk; j++) {
			intragroupbuffer[i][j] = 0;
		}
	}

	MPI_Request* reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * numofnodesingroup);
	MPI_Request* reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * numofnodesingroup);


	// Place MPI_Irecv
	for (int i = 0; i < numofnodesingroup; i++) {
		// Compute source
		int source = numofnodesingroup * groupnumber + i;
		if (source != rank) {
			MPI_Irecv(intragroupbuffer[i], numofitemsineachchunk, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}
	// Place MPI_Isend
	for (int i = 0; i < numofnodesingroup; i++) {
		// Compute destination
		int destination = numofnodesingroup * groupnumber + i;
		if (destination != rank) {
			MPI_Isend(&sendbuf[i * numofitemsineachchunk], numofitemsineachchunk, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);
		}
		else {
			// copy data reduction buffer (intragroupbuffer)
			for (int j = 0; j < numofitemsineachchunk; j++) {
				intragroupbuffer[i][j] = sendbuf[i * numofitemsineachchunk + j];
			}
		}
	}

	//wait for nonblocking receive to complete
	for (int i = 0; i < numofnodesingroup; i++) {
		// Compute destination
		int source = numofnodesingroup * groupnumber + i;
		if (source != rank) {
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	//wait for nonblocking send to complete
	for (int i = 0; i < numofnodesingroup; i++) {
		// Compute destination
		int destination = numofnodesingroup * groupnumber + i;
		if (destination != rank) {
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}

	// execute the reduction operation
	// Could reduce time by pipeline with receiveing data
	float* intragroupreductionresult = (float*)malloc(sizeof(float) * numofitemsineachchunk);
	for (int i = 0; i < numofitemsineachchunk; i++) {
		intragroupreductionresult[i] = 0;
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		for (int j = 0; j < numofitemsineachchunk; j++) {
			intragroupreductionresult[j] = intragroupreductionresult[j] + intragroupbuffer[i][j];
		}
	}

	// free memory
	for (int i = 0; i < numofnodesingroup; i++) {
		free(intragroupbuffer[i]);
	}
	free(intragroupbuffer);
	free(reqsends);
	free(reqrecvs);


	//Step 2: Inter group exchange  /////////////////////////////////////////////////////////////////
	//Prepare buffer to receive data
	// int numofitemsinsecondreduction = NUM_ITEMS_ROUND / size;
	int numofitemsinsecondreduction = NUM_ITEMS_ROUND / size;

	// float **intergroupbuffer = (float**)malloc(sizeof(float*)*numofgroups);
	//intergroupbuffer[i] = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);
	float** intergroupbuffer = (float**)malloc(sizeof(float*) * numofgroups);
	for (int i = 0; i < numofgroups; i++) {
		intergroupbuffer[i] = (float*)malloc(sizeof(float) * numofitemsinsecondreduction);
		for (int j = 0; j < numofitemsinsecondreduction; j++) {
			intergroupbuffer[i][j] = 0; // should carefull for other reduce operation
		}
	}
	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * numofgroups);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * numofgroups);

	// Receive data
	for (int i = 0; i < numofgroups; i++) {
		int source = i * numofnodesingroup + nodenumber;
		MPI_Irecv(intergroupbuffer[i], numofitemsinsecondreduction, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
	}


	// Send data
	float** sendbuf00 = (float**)malloc(sizeof(float*) * numofgroups);
	for (int i = 0; i < numofgroups; i++) {
		sendbuf00[i] = (float*)malloc(sizeof(float) * numofitemsinsecondreduction);
	}

	for (int i = 0; i < numofgroups; i++) {
		int destination = i * numofnodesingroup + nodenumber;

		//prepare data for sendbuf
		for (int j = 0; j < numofitemsinsecondreduction; j++) {
			//read data from intragroupreductionresult
			sendbuf00[i][j] = intragroupreductionresult[j * numofgroups + i];
		}
		if (destination != rank) {
			MPI_Isend(sendbuf00[i], numofitemsinsecondreduction, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);
		}
		else { //copy from buffer
			for (int j = 0; j < numofitemsinsecondreduction; j++) {
				intergroupbuffer[i][j] = sendbuf00[i][j];
			}
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		int source = i * numofnodesingroup + nodenumber;
		if (rank != source) {
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	for (int i = 0; i < numofgroups; i++) {
		int destination = i * numofnodesingroup + nodenumber;
		if (rank != destination) {
			MPI_Wait(&reqsends[i], MPI_STATUSES_IGNORE);
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		free(sendbuf00[i]);
	}
	free(sendbuf00);
	free(reqsends);
	free(reqrecvs);

	//Execute reduction -> change += to reduction function
	// float* intergroupreductionresult = (float*)malloc(sizeof(float) * numofitemsinsecondreduction);
	float* intergroupreductionresult = (float*)malloc(sizeof(float) * numofitemsinsecondreduction);
	for (int i = 0; i < numofitemsinsecondreduction; i++) {
		intergroupreductionresult[i] = 0;
	}
	for (int i = 0; i < numofgroups; i++) {
		for (int j = 0; j < numofitemsinsecondreduction; j++) {
			intergroupreductionresult[j] += intergroupbuffer[i][j];
		}
	}

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
	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * numofgroups);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * numofgroups);
	// float* intergroupbuffer_ = (float*)malloc(sizeof(float) * numofgroups * numofitemsinsecondreduction);
	float* intergroupbuffer_ = (float*)malloc(sizeof(float) * numofgroups * numofitemsinsecondreduction);
	for (int i = 0; i < numofgroups; i++) {
		// Compute source
		int source = numofnodesingroup * i + nodenumber;
		if (source != rank) {
			MPI_Irecv(&intergroupbuffer_[i * numofitemsinsecondreduction], numofitemsinsecondreduction, \
				MPI_FLOAT, source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		// Compute destination
		int destination = numofnodesingroup * i + nodenumber;
		if (destination != rank) {
			MPI_Isend(intergroupreductionresult, numofitemsinsecondreduction, MPI_FLOAT, destination, 1, \
				MPI_COMM_WORLD, &reqsends[i]);
		}
		else {
			//intergroupbuffer_[i] <- intergroupreductionresult (data is on a process so dont need to send)
			for (int j = 0; j < numofitemsinsecondreduction; j++) {
				intergroupbuffer_[i * numofitemsinsecondreduction + j] = intergroupreductionresult[j];
			}
		}
	}

	for (int i = 0; i < numofgroups; i++) {
		// Compute source
		int source = numofnodesingroup * i + nodenumber;
		if (source != rank) {
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	for (int i = 0; i < numofgroups; i++) {
		// Compute destination
		int destination = numofnodesingroup * i + nodenumber;
		if (destination != rank) {
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}

	free(intergroupreductionresult);
	// Step 2: Intra group gather  //////////////////////////////////////////////////////////////////
	//Final result

	reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * numofnodesingroup);
	reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * numofnodesingroup);

	for (int i = 0; i < NUM_ITEMS_ROUND; i++) {
		recvbuf[i] = 0;
	}
	//Allocate buffer for intra group
	// float* bufreorder = (float*)malloc(sizeof(float) * numofgroups * numofitemsinsecondreduction);
	float* bufreorder = (float*)malloc(sizeof(float) * numofgroups * numofitemsinsecondreduction);
	for (int i = 0; i < numofgroups * numofitemsinsecondreduction; i++) {
		int index = i / numofitemsinsecondreduction + numofgroups * (i % numofitemsinsecondreduction);
		// fprintf(stdout, "%d ", index);
		bufreorder[index] = intergroupbuffer_[i];
	}
	// fprintf(stdout, "\n");


	for (int i = 0; i < numofnodesingroup; i++) {
		int source = groupnumber * numofnodesingroup + i;
		int m = numofitemsinsecondreduction * numofgroups;
		if (rank != source) {
			MPI_Irecv(&recvbuf[m * i], numofgroups * numofitemsinsecondreduction, MPI_FLOAT, \
				source, 1, MPI_COMM_WORLD, &reqrecvs[i]);
		}
	}

	for (int i = 0; i < numofnodesingroup; i++) {
		int destination = groupnumber * numofnodesingroup + i;
		if (rank != destination) {
			MPI_Isend(bufreorder, numofgroups * numofitemsinsecondreduction, MPI_FLOAT, \
				destination, 1, MPI_COMM_WORLD, &reqsends[i]);
		}
		else {
			int m = numofitemsinsecondreduction * numofgroups;
			memcpy(&recvbuf[m * i], bufreorder, sizeof(float) * m);
		}
	}

	for (int i = 0; i < numofnodesingroup; i++) {
		int source = groupnumber * numofnodesingroup + i;
		if (rank != source) {
			MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		}
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		int destination = groupnumber * numofnodesingroup + i;
		if (rank != destination) {
			MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
		}
	}

	free(bufreorder);
	free(intergroupbuffer_);
	free(reqrecvs);
	free(reqsends);
	return MPI_SUCCESS;
}


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
static inline void swap(float* arr, int a, int b) {
	float tmp = arr[a];
	arr[a] = arr[b];
	arr[b] = tmp;
	return;
}

int h2r_r(const int groupnumber, const int nodenumber, char* networkshape) { //host name to rank for 2lvfc
	int networkshape_a, networkshape_b;

	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	return groupnumber * networkshape_b + nodenumber;
}