/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:33:06
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-07 23:33:34
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
	
	int groupnumber, nodenumber;
	r2h(rank, d, groupnumber, nodenumber);
	if (rank != h2r(hostname, d))
		program_abort("Computed rank must equal to real rank\n");

	int numofgroups = size/d;
	int numofnodesingroup = d;

	// Load schedule table
	ifstream file;
	std::string filename = "scheduleTable/kout" + to_string(d);
	file.open (filename);


	// reduce memory used: each process hold one line of this table
	string line;
	vector <Int3>*scheduleTable = new vector<Int3> [size];
	for (int i = 0; i < size; i++){
		getline(file, line);
		istringstream iss(line);
		string schedule;
		while (iss >> schedule) { 
			Int3 tmp;
			sscanf(schedule.c_str(), "%hu,%hu,%d,%d", &(tmp.dst), &tmp.src, &(tmp.sendidx), &tmp.recvidx);
			scheduleTable[i].push_back(tmp); 
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
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		start_time = MPI_Wtime();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLGATHER : START	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int numOfStep = d + 1;
	MPI_Request *reqrecvs;
	MPI_Request *reqsends;
	
	for (int step = 0; step < numOfStep; step++){
		if (step == 0){ // Step == 0 send to d nodes
			// Read schedule table first d collumn, process rank i read line i

			// a cell in schetable contain dst destination, dtaIndex place of data
			// See scheduletable.txt
			
			reqrecvs = new MPI_Request[d];
			reqsends = new MPI_Request[d];
			
			for (int i = 0; i < d; i++){
				int destination = scheduleTable[rank][i].dst; 
				int source = scheduleTable[rank][i].src;
				int sendIdx = scheduleTable[rank][i].sendidx*NUM_ITEMS;
				int recvIdx = scheduleTable[rank][i].src*NUM_ITEMS;
				//cout << "From rank " << rank << " destination: " << destination \
						<< " data index: " << sendIdx << endl;
				MPI_Irecv(&allGatherResult[recvIdx], NUM_ITEMS, MPI_FLOAT, source, \
					0, MPI_COMM_WORLD, &reqrecvs[i]);
				MPI_Isend(&allGatherResult[sendIdx], NUM_ITEMS, MPI_FLOAT, destination, \
					0, MPI_COMM_WORLD, &reqsends[i]);
			}

			for (int i = 0; i < d; i++){
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			
#if defined(PRINT_STEP_0)
	// Print allgather result after step 0
	cout << "From rank " << rank << " allgatherresult after step 0:\n\t";
	for (int i = 0; i < NUM_ITEMS*size; i++){
		cout << allGatherResult[i] << "\t";
	}
	cout << endl;
#endif
			delete reqrecvs;
			delete reqsends;
		} else if (step == 1){ //step = 1, then send to d - 1 node
			reqrecvs = new MPI_Request[d - 1];
			reqsends = new MPI_Request[d - 1];

			for (int i = 0; i < d - 1; i++){
				int source = scheduleTable[rank][d + i].src;
				int destination = scheduleTable[rank][d + i].dst;
				int sendIdx = scheduleTable[rank][d + i].sendidx*NUM_ITEMS;
				int recvIdx = scheduleTable[rank][d + i].recvidx*NUM_ITEMS;
				//printf("->> From rank %d, source: %d, dest: %d, sendidx: %d, recvidx: %d\n",\
						rank, source, destination, sendIdx, recvIdx);
				MPI_Irecv(&allGatherResult[recvIdx], NUM_ITEMS, MPI_FLOAT, 
						source, 0, MPI_COMM_WORLD, &reqsends[i]);
				MPI_Isend(&allGatherResult[sendIdx], NUM_ITEMS, MPI_FLOAT, \
						destination, 0, MPI_COMM_WORLD, &reqrecvs[i]);
			}

			for(int i = 0; i < d - 1; i++){
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			for(int i = 0; i < d - 1; i++){
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			delete reqsends;
			delete reqrecvs;
#if defined(PRINT_STEP_1)
	// Print allgather result after step 1
	cout << "From rank " << rank << " allgatherresult after step 1 :\n\t";
	for (int i = 0; i < NUM_ITEMS*size; i++){
		cout << allGatherResult[i] << "\t";
	}
	cout << endl;
#endif
		} else { // send to d nodes with different index
			for (int step = 2; step <= d; step++){
				reqrecvs = new MPI_Request[d];
				reqsends = new MPI_Request[d];

				for (int i = 0; i < d; i++){
					int source = scheduleTable[rank][step*d + i - 1].src;
					int destination = scheduleTable[rank][step*d + i - 1].dst;
					int sendIdx = scheduleTable[rank][step*d + i - 1].sendidx*NUM_ITEMS;
					int recvIdx = scheduleTable[rank][step*d + i - 1].recvidx*NUM_ITEMS;

					MPI_Irecv(&allGatherResult[recvIdx], NUM_ITEMS, MPI_FLOAT, source, \
							0, MPI_COMM_WORLD, &reqrecvs[i]);
					MPI_Isend(&allGatherResult[sendIdx], NUM_ITEMS, MPI_FLOAT, destination,\
							0, MPI_COMM_WORLD, &reqsends[i]);
				}
				
				for(int i = 0; i < d; i++){
					MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
				}
				for (int i = 0; i < d; i++){
					MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
				}
				delete reqrecvs;
				delete reqsends;
			}
		}
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

#if defined(COMPARE_BUILDIN)
	start_time = MPI_Wtime();
	float *allgatherresultlib = (float*)malloc(sizeof(float)*NUM_ITEMS*size);
	MPI_Allgather(data, NUM_ITEMS, MPI_FLOAT, allgatherresultlib, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "%dx%d,%.7lf,%.7lf,%d\n", numofgroups, numofnodesingroup, kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
	}

	// Compare the result
	if (rank == 0){
		for (int i = 0; i < NUM_ITEMS*size; i++){
			if (allGatherResult[i] != allgatherresultlib[i]){
				fprintf(stdout, "%s\n", "Allgather wrong");
				break;
			}
		}
		free(data);
	}
#endif
	/*float * allGatherResultLib = new float[NUM_ITEMS*size];
	MPI_Allgather(data, NUM_ITEMS, MPI_FLOAT, allGatherResultLib, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
	// if (rank ==0 ){
	// 	cout << "lib result:\n";
	// 	for (int i = 0; i < NUM_ITEMS*size; i++){
	// 		cout << allGatherResultLib[i] << "\t";
	// 	}
	// 	cout << endl;
	// }
	
	for (int i = 0; i < NUM_ITEMS*size; i++){
		if (allGatherResultLib[i] != allGatherResult[i]){
			cout << "Allgather wrong\n";
			exit(1);
		}
	}*/

	delete allGatherResult;
	MPI_Finalize();
	return 0;
}