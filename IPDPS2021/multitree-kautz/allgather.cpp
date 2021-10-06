/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:33:06
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-06 15:06:19
 * @ Description:
 */


#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <mpi.h>

#include "helper.hpp"
#include "config.hpp"

using namespace std;
#define RAND_SEED 721311


int main ( int argc, char *argv[] ){
	int rank;
	int size;
	int hostname_len;
	int NUM_ITEMS = 2;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	

	// Calculate degree
	int d = (1 + sqrt(-1 + 4*size))/2;
	
	char hostname[256];
	MPI_Get_processor_name(hostname, &hostname_len);
	char topo[256];
	// Topology optional argument
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--topo")) {
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%s", topo) != 1)) {
				program_abort("Invalid <topology> argument\n");
			} else { // prepare fake hostname for testing on MPICH (without simulation
				r2h_r(rank, d, hostname);
				hostname_len = strlen(hostname) + 1;
				int tmpd;
				sscanf(topo, "k%d", &tmpd);
				if (tmpd != d){
					program_abort("Invalid number of process\n");
				}

#if defined(DEBUG0)
				printf("Rank: %d | Host name: %s | Length: %d\n", rank, hostname, hostname_len);
#endif

			}
		}
		if (!strcmp(argv[i], "--num-item")) {
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%d", &NUM_ITEMS) != 1)) {
				program_abort("Invalid num of items.\n");
			}
		}
	}

#if defined(DEBUG0)
	printf("Hostname: %s | Rank: %d\n", hostname, rank);
#endif

	if (size != d*(d+1)){
		program_abort("Number of process must equal to # node Kautz graph diameter 2\n");
	}

	// Allocate memory for data
	float * data;
	if ((data = new float[NUM_ITEMS]) == NULL) {
		program_abort("Out of memory!");
	}

	// All rank fill the buffer with random data
	srandom(RAND_SEED + rank);
	for (int i = 0; i < NUM_ITEMS; i++) {
		data[i] = (float)(1 + 1.0 * (random() % 9));
	}

#if defined(DEBUG1)
	printf("Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS; i++){
		printf("%.0f\t", data[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
#endif

	// Start the timer
	double start_time;
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		start_time = MPI_Wtime();
	}
	int groupnumber, nodenumber;
	r2h(rank, d, groupnumber, nodenumber);
	if (rank != h2r(hostname, d))
		program_abort("Computed rank must equal to real rank\n");

	int numofgroups = size/d;
	int numofnodesingroup = d;

	// Load schedule table

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	ALLGATHER : START	////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	 ALLGATHER : END	 ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	MPI_Finalize();
	return 0;
}