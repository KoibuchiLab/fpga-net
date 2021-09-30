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

	// Topology optional argument
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--topo")) {
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%s", topo) != 1)) {
				program_abort("Invalid <topology> argument\n");
			} else { // prepare fake hostname for testing
				int g, n;
				r2h_r(rank, topo, hostname);
				hostname_len = strlen(hostname);
				printf("Rank: %d | Host name: %s | Length: %d\n", rank, hostname, hostname_len);
			}
		}
		if (!strcmp(argv[i], "--num-item")) {
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%d", &NUM_ITEMS) != 1)) {
				program_abort("Invalid <topology> argument\n");
			}
		}

	}
	MPI_Finalize();
}
