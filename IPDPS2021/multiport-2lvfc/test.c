#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG 0
#define DEBUG1  // Intra group reduscatter
//#define DEBUG2   // Inter group reducescatter
//#define DEBUG3   // Inter group allgather
//#define DEBUG4   // Intra group allgather

#define RAND_SEED 721311
#define KIM_DATA MPI_DOUBLE
#define KIM_data double

static void program_abort(char* message) {
	fprintf(stderr, "Error: %s", message);
}
void r2h_r(const int rank, const char* topo, char* hostname) {  // rank to host name for 2lvfc
	int numofgroups, numofnodes;
	sscanf(topo, "%dx%d", &numofgroups, &numofnodes);
	sprintf(hostname, "%d_%d", rank / numofnodes, rank % numofnodes);
	return;
}
void r2h(const int rank, const char* networkshape, int* groupnumber, int* nodenumber) {  // rank to host name for 2lvfc
	int networkshape_a, networkshape_b;

	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	*groupnumber = rank / networkshape_b;
	*nodenumber = rank % networkshape_b;
	return;
}

int h2r(const char* hostname, const char* networkshape) {  // host name to rank for 2lvfc
	int groupnumber, nodenumber;
	int networkshape_a, networkshape_b;

	sscanf(hostname, "%d_%d", &groupnumber, &nodenumber);
	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	return groupnumber * networkshape_b + nodenumber;
}
int h2r_r(const int groupnumber, const int nodenumber, const char* networkshape) {  // host name to rank for 2lvfc
	int networkshape_a, networkshape_b;

	sscanf(networkshape, "%d_%d", &networkshape_a, &networkshape_b);
	networkshape_b += 1;
	return groupnumber * networkshape_b + nodenumber;
}

int main(int argc, char* argv[]) {
	int size, rank, NUM_ITEMS;
	struct timeval start, end;
	char hostname[256];
	char topo[256];  // for real MPICH test
	int hostname_len;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	FILE* debugfile;
	char filename[256];
	sprintf(filename, "debugfiles/%d", rank);
	debugfile = fopen(filename, "w");

	MPI_Get_processor_name(hostname, &hostname_len);
	NUM_ITEMS = 338;  // default numitems
	int numofgroups = 3;
	int numofnodesingroup = 5;
	char networkshape[256] = "2_4";
	int groupnumber, nodenumber;  // Hostname define in xml is groupnumber_nodenumber
	r2h(rank, networkshape, &groupnumber, &nodenumber);

	sscanf(networkshape, "%d_%d", &numofgroups, &numofnodesingroup);
	numofgroups++;
	numofnodesingroup++;
	KIM_data* data = (KIM_data*)malloc(sizeof(KIM_data) * NUM_ITEMS * size);
	for (int j = 0; j < NUM_ITEMS * size; j++) {
		data[j] = (KIM_data)(j / NUM_ITEMS + rank * 10);
	}


	MPI_Request* reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * (numofnodesingroup));
	MPI_Request* reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * (numofnodesingroup));
	MPI_Status* statussends = (MPI_Status*)malloc(sizeof(MPI_Status) * numofnodesingroup);
	MPI_Status* statusrecvs = (MPI_Status*)malloc(sizeof(MPI_Status) * numofnodesingroup);


	// Receive data (KIM_data**)malloc(sizeof(KIM_data*)*numofnodesingroup);
	KIM_data** recvbufintra = (KIM_data**)malloc(sizeof(KIM_data*) * numofnodesingroup);
	// KIM_data *tester = (KIM_data*)malloc(sizeof(KIM_data)*numofgroups*NUM_ITEMS);
	// MPI_Request reqtestr, reqtests;
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			recvbufintra[i] = (KIM_data*)malloc(sizeof(KIM_data) * (numofgroups) * NUM_ITEMS);
			for (int k = 0; k < numofgroups * NUM_ITEMS; k++) {
				recvbufintra[i][k] = -1;
			}
		}
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			int source = h2r_r(groupnumber, i, networkshape);
			int numitem = numofgroups * NUM_ITEMS;
			fprintf(debugfile, "Wait to receive %d items from node %d\n", numitem, source);
			MPI_Irecv(&recvbufintra[i][0], numitem, KIM_DATA, source, 0, MPI_COMM_WORLD, &(reqrecvs[i]));
			// MPI_Irecv(tester, numitem, KIM_DATA, source, 0, MPI_COMM_WORLD, &reqtestr);
		}
	}

	// Prepare data for intra group transfer
	
	KIM_data** sendbufintra = (KIM_data**)malloc(sizeof(KIM_data*) * numofgroups * NUM_ITEMS);
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			sendbufintra[i] = (KIM_data*)malloc(sizeof(KIM_data) * (numofgroups) * NUM_ITEMS);
		}
	}
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			fprintf(debugfile, "Send data from %d_%d to %d_%d: ", groupnumber, nodenumber, groupnumber, i);
			for (int j = 0; j < numofgroups; j++) {
				// fprintf(debugfile, "%d_%d ", j, i); // Index of data
				// prepare sendbuf
				// memcpy(&sendbufintra[NUM_ITEMS*j], )

				int index = h2r_r(j, i, networkshape);
				fprintf(debugfile, "Index: %d Data: ", index);
				for (int k = 0; k < NUM_ITEMS; k++) {
					sendbufintra[i][j * NUM_ITEMS + k] = data[index * NUM_ITEMS + k];
					fprintf(debugfile, "%.0f ", sendbufintra[i][j * NUM_ITEMS + k]);
					// fprintf(debugfile, "%d ", sendbufintra[j * NUM_ITEMS + k]);
				}
			}
			int dest = h2r_r(groupnumber, i, networkshape);
			MPI_Isend(sendbufintra[i], NUM_ITEMS * numofgroups, KIM_DATA, dest, 0, MPI_COMM_WORLD, &(reqsends[i]));
			// MPI_Isend(sendbufintra, NUM_ITEMS * numofgroups, KIM_DATA, dest, 0, MPI_COMM_WORLD, &reqtests);
			fprintf(debugfile, "\n sizeof float: %ld, sizeof MPI_FLOAT %ld\n", sizeof(float), sizeof(MPI_FLOAT));
		}
	}
	fprintf(debugfile, "Status error code: ");
	// MPI_Wait(&reqtestr, MPI_STATUS_IGNORE);
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			MPI_Wait(&(reqsends[i]), &(statussends[i]));
			fprintf(debugfile, "%d ", (statussends[i]).MPI_SOURCE);
			// fprintf(debugfile, "%ld ", statusrecvs[i].count);
		}
	}
	
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			MPI_Wait(&reqrecvs[i], &statusrecvs[i]);
			fprintf(debugfile, "Node %d_%d receive data from %d_%d: ", groupnumber, nodenumber, groupnumber, i);
			for (int j = 0; j < numofgroups; j++) {
				fprintf(debugfile, "%d_%d ", j, nodenumber);
			}
			fprintf(debugfile, "\nData received: ");
			for (int j = 0; j < numofgroups*NUM_ITEMS; j++) {
				fprintf(debugfile, "%.0f ", recvbufintra[i][j]);
				// fprintf(debugfile, "%d ", recvbufintra[i][j]);
			}
			fprintf(debugfile, "\n");
		}
	}
	
	fprintf(debugfile, "\n");
	// fprintf(debugfile, "Test data ");
	// for (int i = 0; i < numofgroups*NUM_ITEMS; i++){
	// 	fprintf(debugfile, "%.0f ", tester[i]);
	// }
	// fprintf(debugfile, "\n");
	fprintf(debugfile, "numofgroup: %d, numofnode: %d, groupnumber: %d, nodenumber%d\n", numofgroups, numofnodesingroup, groupnumber, nodenumber);
	free(sendbufintra);


	fprintf(debugfile, "From rank: %d|\t recvbufintra\n", rank);
	for (int i = 0; i < numofnodesingroup; i++) {
		if (i != nodenumber) {
			for (int j = 0; j < numofgroups * NUM_ITEMS; j++) {
				fprintf(debugfile, "\t\t%.0f ", recvbufintra[i][j]);
				// fprintf(debugfile, "\t\t%d", recvbufintra[i][j]);
			}
			fprintf(debugfile, "\n");
		}
	}
	fprintf(debugfile, "\n");

	fclose(debugfile);
	free(reqsends);
	free(reqrecvs);

	MPI_Finalize();
}
