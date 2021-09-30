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


void r2h_r(int rank, char* topo, char* hostname){ // rank to host name for 2lvfc
	char networkshape[256];
	strcpy(networkshape, topo);
	char * token = strtok(networkshape, "x");
	token = strtok(NULL, "x");
	int networkshape_b = atoi(token);
	sprintf(hostname, "%d_%d", rank / networkshape_b, rank % networkshape_b);
	return;
}
int main(int argc, char *argv[])
{
	int size, rank, NUM_ITEMS;
	struct timeval start, end;
	char hostname[256];
	char topo[256];//for real MPICH test
	int hostname_len;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname,&hostname_len);
	NUM_ITEMS = 500; //default numitems
	int numofgroups = 3;
	int numofnodesingroup = 4;
	int nodenumber = 0;
	int numofitemsinsecondreduction = 2;
	MPI_Request *reqsends = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	MPI_Request *reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*numofgroups);
	float *intergroupreductionresult = (float*)malloc(sizeof(float)*numofitemsinsecondreduction);
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
	free(reqsends);
	free(reqrecvs);

	MPI_Finalize();
}
