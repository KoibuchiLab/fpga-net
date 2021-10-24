/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:33:06
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-24 19:51:59
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
	char algo = CONGESTION;
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
			if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%d", &NUM_ITEMS) != 1)) {
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
	if ((data = new float[NUM_ITEMS]) == NULL) {
		program_abort("Allocate fail!");
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
	
	int groupnumber, nodenumber;
	r2h(rank, d, groupnumber, nodenumber);
	if (rank != h2r(hostname, d))
		program_abort("Computed rank must equal to real rank\n");

	int numofgroups = size/d;
	int numofnodesingroup = d;

	vector <Int3>scheduleTable;
	vector <int>*childParent;
	if (algo == COMBINE){
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
	} else {
		// Load schedule table
		ifstream file;
		std::string filename = "scheduleTable/kout" + to_string(d);
		file.open (filename);
		// reduce memory used: each process hold one line of this table
		string line;
		

		for (int i = 0; i < size; i++){
			getline(file, line);
			if (i == rank){
				istringstream iss(line);
				string schedule;
				while (iss >> schedule) { 
					Int3 tmp;
					sscanf(schedule.c_str(), "%d,%d,%d,%d", &(tmp.dst), &tmp.src, &(tmp.sendidx), &tmp.recvidx);
					scheduleTable.push_back(tmp); 
				}
			}
		}
	}

#if defined(SCHEDULE_TABLE)
	if(rank == 0){
		for (int i = 0; i < size; i++){
			for(auto j: scheduleTable[i]){
				cout << j.dst << "," << j.src << "," << j.sendidx << "," << j.recvidx << "\t";
			}
			cout << endl;
		}
	}
#endif

	// allocate memory for result
	float *allGatherResult = new float[NUM_ITEMS*size]();
	// copy local data to result
	for (int i = 0; i < NUM_ITEMS; i++){
		allGatherResult[rank*NUM_ITEMS + i] = data[i];
	}
	float **recvbuf;
	float *sendbuf;
	if(algo == COMBINE){
		recvbuf = new float*[d];
		for (int i = 0; i < d; i++){
			recvbuf[i] = new float[d*NUM_ITEMS];
		}
		sendbuf = new float[d*NUM_ITEMS];
	}
	MPI_Request *reqrecvs;
	MPI_Request *reqsends;
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		start_time = MPI_Wtime();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLTOALL : START	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	switch(algo) {
		case MULTITREE  :{ // Allgather algorithm is multi tree
			;
			break; //optional
		
		} case CONGESTION: {
			;
			break; //optional
		} case COMBINE:{
			// Step o send data of this node to all child nodes

		} default : //Optional
			break;											
	}
	
#if defined(PRINT_RESULT)
	// Print allgather result after step 
	if (rank == 0){
		cout << "From rank " << rank << " allgatherresult:\n\t";
		for (int i = 0; i < NUM_ITEMS*size; i++){
			cout << allGatherResult[i] << "\t";
		}
		cout << endl;
	}
	
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	 ALLGATHER : END	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// Stop timer
	MPI_Barrier(MPI_COMM_WORLD);
    
	double kimrdtime = MPI_Wtime() - start_time;
    if ((0 == rank)) {
		fprintf(stdout, "k%d,%.7lf,%d\n", d, kimrdtime, NUM_ITEMS);
	}
#if defined(COMPARE_BUILDIN)
	start_time = MPI_Wtime();
	float *allgatherresultlib = (float*)malloc(sizeof(float)*NUM_ITEMS*size);
	MPI_Allgather(data, NUM_ITEMS, MPI_FLOAT, allgatherresultlib, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "k%d,%.7lf,%.7lf,%d\n", d, kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
	}

	// Compare the result
	if (rank == 0){
		for (int i = 0; i < NUM_ITEMS*size; i++){
			if (allGatherResult[i] != allgatherresultlib[i]){
				fprintf(stdout, "%s\n", "Allgather wrong");
				break;
			}
		}
	}
#endif
	delete data;
	delete allGatherResult;
	MPI_Finalize();
	return 0;
}