/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:33:06
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-04-14 08:50:36
 * @ Description:
 */

#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#include "../../simgrid-3.28/install/include/smpi/smpi.h"
#include "helper.hpp"
#include "config.hpp"


using namespace std;
#define RAND_SEED 721311


int main ( int argc, char *argv[] ){
    // {int i=0;
    // while (0 == i)
    //     sleep(5);}
	int rank;
	int size;
	int hostname_len;
	size_t NUM_ITEMS = 1011;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	char algo = COMBINE;
	double dblasttimer;
	// Calculate degree
	int d = (1 + sqrt(-1 + 4*size))/2;
	char hostname[256];
	MPI_Get_processor_name(hostname, &hostname_len);
	char topo[256];
	// Topology optional argument
	for (int i = 1; i < argc; i++){
		if (!strcmp(argv[i], "--topo")){
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
		if (!strcmp(argv[i], "--num-item")){
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%ld", &NUM_ITEMS) != 1)) {
				program_abort("Invalid num of items.\n");
			}
		}
		if (!strcmp(argv[i], "--algorithm")){
			if (i + 1 <= argc) {
				if (!strcmp(argv[i + 1], "mttree")){
					algo = MULTITREE;
				}
				if (!strcmp(argv[i + 1], "conges")){
					algo = CONGESTION;
				}
				if (!strcmp(argv[i + 1], "cmb")){
					algo = COMBINE;
				}
			} else {
				program_abort("Enter algorithm.\n");
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
	data = (float*)SMPI_SHARED_MALLOC(sizeof(float) * NUM_ITEMS * size);
	// if ((data = new float[NUM_ITEMS*size]) == NULL) {
	// 	program_abort("Allocate fail!");
	// }

	// All rank fill the buffer with random data
	size_t DATA_SIZE = NUM_ITEMS*size;
	// printf("%ld\n", DATA_SIZE);
	// for (size_t i = 0; i < DATA_SIZE; i++) {
    //         //data[i] = (float)((rank*1000 + i));
	// 	    data[i] =  i; //(float)(1 + 1.0 * (random() % 9));
	// }                                                                                                        
#if defined(DEBUG1)
	printf("Data from rank %d: ", rank);
	for (size_t i = 0; i < DATA_SIZE; i++){
		printf("%.0f\t", data[i]);
	}
	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
#endif

	// Start the timer
	double start_time;
	
	int groupnumber, nodenumber;
	r2h(rank, d, groupnumber, nodenumber);
	if (rank != h2r(hostname, d))
		program_abort("Computed rank must equal to real rank\n");

	int numofgroups = size/d;
	int numofnodesingroup = d;

	vector <int>*childParent;
	// Open file
	ifstream file;
	string line;
	std::string filename = "scheduleTable/kcmb" + to_string(d);
	file.open (filename);
	int tmp;
	childParent = new vector <int> [size];
	for (int i = 0; i < size; i++){
		getline(file, line);
		istringstream iss(line);
		string alist;
		for(int j = 0; j < 2*d + 1; j++){
			iss >> tmp;
			childParent[i].push_back(tmp);
		}
	}
	file.close();

	// allocate memory for result NUM_ITEMS*size
	float* result = data;
	// copy local data to result
	memcpy(&result[rank*NUM_ITEMS], &data[rank*NUM_ITEMS], NUM_ITEMS*sizeof(float));
	
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		start_time = MPI_Wtime();
	}
	// if ((0 == rank)) {
	// 	fprintf(stdout, "alo 0\n");
	// }
	// double kstart, step1, step2, preparebuf, createrecvcode, createsendcode, communicate; 
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLTOALL : START	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	switch(algo) {
		case MULTITREE  :{ 
			// Step 0 send all the neccessary information to all neighbors
			// float *sendbuf = (float*)SMPI_SHARED_MALLOC(sizeof(float)*(d + 1)*NUM_ITEMS);
			float* sendbuf = data;

			// float* recvbuf = (float*)SMPI_SHARED_MALLOC(sizeof(float) * (d + 1) * NUM_ITEMS);
			float* recvbuf = sendbuf;
			MPI_Request *reqsends = new MPI_Request[d];
			MPI_Request *reqrecvs = new MPI_Request[d];

			// for all child: 
			for (int i = 0; i < d; i++){
				int aChild = childParent[rank][i];
				int aParent = childParent[rank][d + i];
				int duplicateIdx = childParent[aChild][0];
				//receive data from parents
				if (aParent != childParent[rank][0]){
					MPI_Irecv(recvbuf, (d + 1)*NUM_ITEMS, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
				} else {
					MPI_Irecv(recvbuf, d*NUM_ITEMS, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
				}

				// Prepare sendbuffer
				// copy data of child of child to sendbuf
				if (rank != duplicateIdx){ // normal copy
					for (int j = 0; j < d; j++){
						memcpy(&sendbuf[j*NUM_ITEMS], &data[childParent[aChild][j] * NUM_ITEMS], NUM_ITEMS*sizeof(float));
					}
					memcpy(&sendbuf[d*NUM_ITEMS], &data[aChild*NUM_ITEMS], NUM_ITEMS*sizeof(float));

					MPI_Isend(sendbuf, (d + 1)*NUM_ITEMS, MPI_FLOAT, aChild, 0, MPI_COMM_WORLD, &reqsends[i]);
				} else { // copy with adjust index
					for (int j = 0; j < d - 1; j++){
						memcpy(&sendbuf[j*NUM_ITEMS], &data[childParent[aChild][j + 1] * NUM_ITEMS], NUM_ITEMS*sizeof(float));
					}
					memcpy(&sendbuf[(d - 1)*NUM_ITEMS], &data[aChild*NUM_ITEMS], NUM_ITEMS*sizeof(float));

					MPI_Isend(sendbuf, d*NUM_ITEMS, MPI_FLOAT, aChild, 0, MPI_COMM_WORLD, &reqsends[i]);
				}
				
			}
			
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
				// Copy data to final result
				int duplicateIdx = childParent[rank][0];
				int cpyIdx = childParent[rank][d + i];
				if(cpyIdx != duplicateIdx){ //normal copy
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf[d*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				} else { // copy with adjust index
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf[(d - 1)*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				}
			}
			
			delete reqrecvs;
			delete reqsends;
			break; //optional
		
		} case CONGESTION: {
			// Step 0 send all the neccessary information to all neighbors
			// float *sendbuf = (float*)SMPI_SHARED_MALLOC(sizeof(float)*(d + 1)*NUM_ITEMS);
			float* sendbuf = data;


			// float* recvbuf = (float*)SMPI_SHARED_MALLOC(sizeof(float) * (d + 1) * NUM_ITEMS);
			float* recvbuf = sendbuf;


			MPI_Request *reqsends = new MPI_Request[d];
			MPI_Request *reqrecvs = new MPI_Request[d];

			// for all child: 
			for (int i = 0; i < d; i++){
				int aChild = childParent[rank][i];
				int aParent = childParent[rank][d + i];
				int duplicateIdx = childParent[aChild][0];
				//receive data from parents
				if (aParent != childParent[rank][0]){
					MPI_Irecv(recvbuf, (d + 1)*NUM_ITEMS, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
				} else {
					MPI_Irecv(recvbuf, d*NUM_ITEMS, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
				}

				// Prepare sendbuffer
				// copy data of child of child to send buf
				if (rank != duplicateIdx){ // normal copy
					for (int j = 0; j < d; j++){
						memcpy(&sendbuf[j*NUM_ITEMS], &data[childParent[aChild][j] * NUM_ITEMS], NUM_ITEMS*sizeof(float));
					}
					memcpy(&sendbuf[d*NUM_ITEMS], &data[aChild*NUM_ITEMS], NUM_ITEMS*sizeof(float));

					MPI_Isend(sendbuf, (d + 1)*NUM_ITEMS, MPI_FLOAT, aChild, 0, MPI_COMM_WORLD, &reqsends[i]);
				} else { // copy with adjust index
					for (int j = 0; j < d - 1; j++){
						memcpy(&sendbuf[j*NUM_ITEMS], &data[childParent[aChild][j + 1] * NUM_ITEMS], NUM_ITEMS*sizeof(float));
					}
					memcpy(&sendbuf[(d - 1)*NUM_ITEMS], &data[aChild*NUM_ITEMS], NUM_ITEMS*sizeof(float));

					MPI_Isend(sendbuf, d*NUM_ITEMS, MPI_FLOAT, aChild, 0, MPI_COMM_WORLD, &reqsends[i]);
				}
				
			}
			
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
				// can place send and receive for the next step here

				// Copy data to final result
				int duplicateIdx = childParent[rank][0];
				int cpyIdx = childParent[rank][d + i];
				if(cpyIdx != duplicateIdx){ //normal copy
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf[d*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				} else { // copy with adjust index
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf[(d - 1)*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				}
			}
			for (int i =0; i < d; i++){
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}	
			delete reqsends;
			delete reqrecvs;
			break; //optional
		} case COMBINE:{

			MPI_Request *reqsends = new MPI_Request[d];
			MPI_Request *reqrecvs = new MPI_Request[d];
			// float *sendbuf1 = (float*)SMPI_SHARED_MALLOC(sizeof(float)*(d + 1)*NUM_ITEMS);
			float* sendbuf1 = data;

			// float* recvbuf1 = (float*)SMPI_SHARED_MALLOC(sizeof(float) * (d + 1) * NUM_ITEMS);
			float* recvbuf1 = sendbuf1;
			
			for (int i = 0; i < d; i++){
				int aChild = childParent[rank][i];
				int aParent = childParent[rank][d + i];
				int duplicateIdx = childParent[aChild][0];
				//receive data from parents
				int tag = aParent;
				for (int j = 0; j < d; j++){
					tag += childParent[aParent][d + j];
				}
				if (aParent != childParent[rank][0]){
					// cout << "From rank: " << rank << " wait to receive from: " << aParent << " tag "<< tag << endl;
					MPI_Irecv(recvbuf1, (d + 1)*NUM_ITEMS, MPI_FLOAT, aParent, tag, MPI_COMM_WORLD, &reqrecvs[i]);
				} else {
					// cout << "From rank: " << rank << " wait to receive from: " << aParent << " tag "<< 1 << endl;
					MPI_Irecv(recvbuf1, d*NUM_ITEMS, MPI_FLOAT, aParent, 1, MPI_COMM_WORLD, &reqrecvs[i]);
				}
			}
			
			for (int i = 0; i < d; i++){
				// Prepare sendbuffer
				// copy data of child of child to send buf
				int aChild = childParent[rank][i];
				int duplicateIdx = childParent[aChild][0];
				int tag = rank;
				if (rank != duplicateIdx){ // normal copy
					for (int j = 0; j < d; j++){
						memcpy(&sendbuf1[j*NUM_ITEMS], &data[childParent[aChild][j] * NUM_ITEMS], NUM_ITEMS*sizeof(float));
						tag += childParent[rank][d + j];
					}
					memcpy(&sendbuf1[d*NUM_ITEMS], &data[aChild*NUM_ITEMS], NUM_ITEMS*sizeof(float));
					// cout << "From rank: " << rank << " send to: " << aChild << " tag "<< tag << endl;
					MPI_Isend(sendbuf1, (d + 1)*NUM_ITEMS, MPI_FLOAT, aChild, tag, MPI_COMM_WORLD, &reqsends[i]);
				} else { // copy with adjust index
					for (int j = 0; j < d - 1; j++){
						memcpy(&sendbuf1[j*NUM_ITEMS], &data[childParent[aChild][j + 1] * NUM_ITEMS], NUM_ITEMS*sizeof(float));
					}
					memcpy(&sendbuf1[(d - 1)*NUM_ITEMS], &data[aChild*NUM_ITEMS], NUM_ITEMS*sizeof(float));
					// cout << "From rank: " << rank << " send to: " << aChild << " tag "<< 1 << endl;
					MPI_Isend(sendbuf1, d*NUM_ITEMS, MPI_FLOAT, aChild, 1, MPI_COMM_WORLD, &reqsends[i]);
				}
				
			}
			
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			// if ((0 == rank)) {
			// 	fprintf(stdout, "alo 3\n");
			// }
			for (int i = 0; i < d; i++){
				// Copy data to final result
				int duplicateIdx = childParent[rank][0];
				int cpyIdx = childParent[rank][d + i];
				if(cpyIdx != duplicateIdx){ //normal copy
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf1[d*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				} else { // copy with adjust index
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf1[(d - 1)*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				}
			}
            
			delete reqrecvs;
			delete reqsends;
			
		} default : //Optional
			break;											
	}
	

	
#if defined(PRINT_RESULT)
	// Print allgather result after step 
	if (rank == 0){
		cout << "From rank " << rank << " allgatherresult:\n\t";
		for (size_t i = 0; i < DATA_SIZE; i++){
			cout << result[i] << "\t";
		}
		cout << endl;
	}
	
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	    ALLTOALL : END	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// Stop timer
	MPI_Barrier(MPI_COMM_WORLD);
	
	double kimrdtime = MPI_Wtime() - start_time;
	if ((0 == rank)) {
		fprintf(stdout, "k%d,%.7lf,%ld\n", d, kimrdtime, NUM_ITEMS);
	}
#if defined(COMPARE_BUILDIN)
	start_time = MPI_Wtime();
	float *resultlib = (float*)malloc(sizeof(float)*DATA_SIZE);
	MPI_Alltoall(data, NUM_ITEMS, MPI_FLOAT, resultlib, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "k%d,%.7lf,%.7lf,%d\n", d, kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
	}

	// Compare the result
	if (rank == 0){
		for (size_t i = 0; i < DATA_SIZE; i++){
			if ((result[i]) != (resultlib[i])){
				cout << "Index " << i << " kim: " << result[i] << " lib " << resultlib[i] << endl;
				fprintf(stdout, "%s\n", "Alltoall wrong");
				break;
			}
		}
	}
#endif
	// if (rank != -10) {
	// 	printf("Step 1: %f, Step2: %f\n", step1 - kstart, step2 - step1);
	// }
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	
	return 0;
}