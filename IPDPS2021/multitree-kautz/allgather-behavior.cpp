/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:33:06
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-14 21:47:17
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

#include "../../simgrid-3.28/install/include/smpi/smpi.h"
#include "helper.hpp"
#include "config.hpp"


using namespace std;
#define RAND_SEED 721311

int main ( int argc, char *argv[] ){
	int rank;
	int size;
	int hostname_len;
	unsigned int NUM_ITEMS = 2;
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
	for (unsigned int i = 1; i < argc; i++){
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
	if ((data =  (float*)SMPI_SHARED_MALLOC(sizeof(float)*NUM_ITEMS)) == NULL) {
		program_abort("Out of memory!");
	}

	// All rank fill the buffer with random data
	srandom(RAND_SEED + rank);
	for (unsigned int i = 0; i < NUM_ITEMS; i++) {
		data[i] = (float)(1 + 1.0 * (random() % 9));
	}

#if defined(DEBUG1)
	printf("Data from rank %d: ", rank);
	for (unsigned int i = 0; i < NUM_ITEMS; i++){
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
		for (unsigned int i = 0; i < size; i++){
			getline(file, line);
			istringstream iss(line);
			string alist;
			for(unsigned int j = 0; j < 2*d + 1; j++){
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

		for (unsigned int i = 0; i < size; i++){
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
		for (unsigned int i = 0; i < size; i++){
			for(auto j: scheduleTable[i]){
				cout << j.dst << "," << j.src << "," << j.sendidx << "," << j.recvidx << "\t";
			}
			cout << endl;
		}
	}
#endif

	// allocate memory for result
	float *allGatherResult = (float*) SMPI_SHARED_MALLOC(sizeof(float)*NUM_ITEMS*size); //new float[NUM_ITEMS*size]();
	// copy local data to result
	for (unsigned int i = 0; i < NUM_ITEMS; i++){
		allGatherResult[rank*NUM_ITEMS + i] = data[i];
	}
	float **recvbuf;
	float *sendbuf;
	if(algo == COMBINE){
		recvbuf = new float*[d];
		for (int i = 0; i < d; i++){
			recvbuf[i] = (float*)SMPI_SHARED_MALLOC(sizeof(float)*d*NUM_ITEMS);//new float[d*NUM_ITEMS];
		}
		sendbuf = new float[d*NUM_ITEMS];
	}
	MPI_Request *reqrecvs;
	MPI_Request *reqsends;
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		start_time = MPI_Wtime();
	}
	double tstep0, tstep1comm, tstep1copy;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLGATHER : START	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	switch(algo) {
		case MULTITREE  :{ // Allgather algorithm is multi tree
			int numOfStep = d + 1;
			
			for (int step = 0; step < numOfStep; step++){
				if (step == 0){ // Step == 0 send to d nodes
					// Read schedule table first d collumn, process rank i read line i

					// a cell in schetable contain dst destination, dtaIndex place of data
					// See scheduletable.txt
					
					reqrecvs = new MPI_Request[d];
					reqsends = new MPI_Request[d];
					
					for (int i = 0; i < d; i++){
						int destination = scheduleTable[i].dst; 
						int source = scheduleTable[i].src;
						unsigned int sendIdx = scheduleTable[i].sendidx*NUM_ITEMS;
						unsigned int recvIdx = scheduleTable[i].src*NUM_ITEMS;
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
		for (unsigned int i = 0; i < NUM_ITEMS*size; i++){
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
						int source = scheduleTable[d + i].src;
						int destination = scheduleTable[d + i].dst;
						unsigned int sendIdx = scheduleTable[d + i].sendidx*NUM_ITEMS;
						unsigned int recvIdx = scheduleTable[d + i].recvidx*NUM_ITEMS;
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
		for (unsigned int i = 0; i < NUM_ITEMS*size; i++){
			cout << allGatherResult[i] << "\t";
		}
		cout << endl;
	#endif
				} else { // send to d nodes with different index
					reqrecvs = new MPI_Request[d];
					reqsends = new MPI_Request[d];

					for (int i = 0; i < d; i++){
						int source = scheduleTable[step*d + i - 1].src;
						int destination = scheduleTable[step*d + i - 1].dst;
						unsigned int sendIdx = scheduleTable[step*d + i - 1].sendidx*NUM_ITEMS;
						unsigned int recvIdx = scheduleTable[step*d + i - 1].recvidx*NUM_ITEMS;

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
			break; //optional
		
		} case CONGESTION: {
			
			// Step == 0 send to d nodes
				// Read schedule table first d collumn, process rank i read line i

				// a cell in schetable contain dst destination, dtaIndex place of data
				// See scheduletable.txt
				
			reqrecvs = new MPI_Request[d];
			reqsends = new MPI_Request[d];
			
			for (int i = 0; i < d; i++){
				int destination = scheduleTable[i].dst; 
				int source = scheduleTable[i].src;
				unsigned int sendIdx = scheduleTable[i].sendidx*NUM_ITEMS;
				unsigned int recvIdx = scheduleTable[i].src*NUM_ITEMS;
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
			delete reqrecvs;
			delete reqsends;
				
#if defined(PRINT_STEP_0)
	// Print allgather result after step 0
	cout << "From rank " << rank << " allgatherresult after step 0:\n\t";
	for (unsigned int i = 0; i < NUM_ITEMS*size; i++){
		cout << allGatherResult[i] << "\t";
	}
	cout << endl;
#endif
			
			// step 1: the remaining nodes
			reqrecvs = new MPI_Request[size];
			reqsends = new MPI_Request[size];

			for (int i = d; i < size - 1; i++){
				int source = scheduleTable[i].src;
				unsigned int recvIdx = scheduleTable[i].recvidx*NUM_ITEMS;

				MPI_Irecv(&allGatherResult[recvIdx], NUM_ITEMS, MPI_FLOAT, source, \
						0, MPI_COMM_WORLD, &reqrecvs[i]);
			}
			for (int i = d; i < size - 1; i++){
				int destination = scheduleTable[i].dst;
				unsigned int sendIdx = scheduleTable[i].sendidx*NUM_ITEMS;

				MPI_Isend(&allGatherResult[sendIdx], NUM_ITEMS, MPI_FLOAT, destination,\
						0, MPI_COMM_WORLD, &reqsends[i]);
			}
			
			for (int i = d; i < size - 1; i++){
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			for (int i = d; i < size - 1; i++){
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			delete reqrecvs;
			delete reqsends;
				
			break; //optional
		} case COMBINE:{
			// Step o send data of this node to all child nodes
			
#if defined(TIME_FOR_EACH_STEP_C)
	// MPI_Barrier(MPI_COMM_WORLD);
	
	if (rank == 0){
		dblasttimer = MPI_Wtime();
	}
#endif
			reqrecvs = new MPI_Request[d];
			reqsends = new MPI_Request[d];
			int source, destination;
            unsigned int sendidx, recvidx;
			for(int i = 0; i < d; i++){
				destination  = childParent[rank][i];
				source = childParent[rank][d + i];
				sendidx = rank*NUM_ITEMS;
				recvidx = childParent[rank][d + i]*NUM_ITEMS;
				MPI_Irecv(&sendbuf[i], NUM_ITEMS, MPI_FLOAT, source, 0, \
						MPI_COMM_WORLD, &reqrecvs[i]);
				MPI_Isend(&allGatherResult[sendidx], NUM_ITEMS, MPI_FLOAT, destination, 0, \
						MPI_COMM_WORLD, &reqsends[i]);
			}
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			delete reqsends;
			delete reqrecvs;
#if defined(TIME_FOR_EACH_STEP_C)
	// MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		//fprintf(stdout, "%.7lf\tTime for step 0\n", MPI_Wtime() - dblasttimer);
		tstep0 = MPI_Wtime() - dblasttimer;
		dblasttimer = MPI_Wtime();
	}
#endif
			
#if defined(PRINT_STEP_0)
	// Print allgather result after step 0
	cout << "From rank " << rank << " allgatherresult after step 0:\n\t";
	for (unsigned int i = 0; i < NUM_ITEMS*size; i++){
		cout << allGatherResult[i] << "\t";
	}
	cout << endl;
#endif
			// Step 1 send receives informantion to all child node
			reqrecvs = new MPI_Request[d];
			reqsends = new MPI_Request[d];
			
			unsigned int duplicateIdx = childParent[rank][2*d];

			//prepare sendbuf
			/*for (int j = 0; j < d; j++){
				for (int k = 0; k < NUM_ITEMS; k++){
					sendbuf[j*NUM_ITEMS + k] = allGatherResult[childParent[rank][d + j]*NUM_ITEMS + k];
				}
			}*/
			int tmpi;


			// send and receive message
			for(int i = 0; i < d; i++){
				source = childParent[rank][d + i];
				if (source == duplicateIdx){
					MPI_Irecv(recvbuf[i], NUM_ITEMS*(d - 1), MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
				} else {
					MPI_Irecv(recvbuf[i], NUM_ITEMS*d, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
				}

				destination  = childParent[rank][i];
				if(destination != duplicateIdx){
					MPI_Isend(sendbuf, NUM_ITEMS*d, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);					
				} else {
					tmpi = i;
				}
			}
			float *nsendbuf = (float*)SMPI_SHARED_MALLOC(sizeof(float)*(d)*NUM_ITEMS);
			
			// send the message at the duplicate index
			bool detectduplicate = false;
			for (int j = 0; j < d; j++){
				if (!detectduplicate){
					if(childParent[rank][d + j] == duplicateIdx){
						detectduplicate = true;
						continue;
					}
                    memcpy(&nsendbuf[j*NUM_ITEMS], &allGatherResult[childParent[rank][d + j]*NUM_ITEMS], sizeof(float)*NUM_ITEMS);
					// for (int k = 0; k < NUM_ITEMS; k++)
					// 	nsendbuf[j*NUM_ITEMS + k] = allGatherResult[childParent[rank][d + j]*NUM_ITEMS + k];
				} else {
					// for (int k = 0; k < NUM_ITEMS; k++)
					// 	nsendbuf[(j - 1)*NUM_ITEMS + k] = allGatherResult[childParent[rank][d + j]*NUM_ITEMS + k];
                    memcpy(&nsendbuf[(j - 1)*NUM_ITEMS], &allGatherResult[childParent[rank][d + j]*NUM_ITEMS], sizeof(float)*NUM_ITEMS);
				}
			}
			MPI_Isend(nsendbuf, NUM_ITEMS*(d - 1), MPI_FLOAT, duplicateIdx, 0, MPI_COMM_WORLD, &reqsends[tmpi]);

			//copy back
			for (int i = 0; i < d; i++){
				recvidx = childParent[rank][d + i]*NUM_ITEMS;
                memcpy(&allGatherResult[recvidx], &sendbuf[i*NUM_ITEMS], sizeof(float)*NUM_ITEMS);
				// for (int j = 0; j < NUM_ITEMS; j++){
				// 	allGatherResult[recvidx + j] = sendbuf[i*NUM_ITEMS + j];
				// }
			}
			
			int *whichData = new int[d];
			// wait for send and reseive complete and copy buffer to final result
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			
			
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
#if defined(TIME_FOR_EACH_STEP_C)
	// MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		tstep1comm = MPI_Wtime() - dblasttimer;
		//fprintf(stdout, "%.7lf\tSend receive done\n", MPI_Wtime() - dblasttimer);
		
		dblasttimer = MPI_Wtime();
	}
#endif
			for (int i = 0; i < d; i++){
				// copy buffer to final result
				// find the data source
				
				// copy the source of data from its schedule 
				for (int j = 0; j < d; j++){
					whichData[j] = childParent[childParent[rank][d + i]][d + j];
				}
				if(childParent[rank][d + i] != duplicateIdx){
					for (int j = 0; j < d; j++){
						// for (int k = 0; k < NUM_ITEMS; k++){
						// 	allGatherResult[whichData[j]*NUM_ITEMS + k] = recvbuf[i][j*NUM_ITEMS + k];
						// }
                        memcpy(&allGatherResult[whichData[j]*NUM_ITEMS], &recvbuf[i][j*NUM_ITEMS], sizeof(float)*NUM_ITEMS);
					}
				} else {
					detectduplicate = false;
					for (int j = 0; j < d; j++){
						if (whichData[j] == childParent[duplicateIdx][2*d]){
							detectduplicate = true;
							continue;
						}
						
						if (!detectduplicate){//Normal copy							
							// for (int k = 0; k < NUM_ITEMS; k++){
							// 	allGatherResult[whichData[j]*NUM_ITEMS + k] = recvbuf[i][(j)*NUM_ITEMS + k];
							// }
                            memcpy(&allGatherResult[whichData[j]*NUM_ITEMS], &recvbuf[i][(j)*NUM_ITEMS], sizeof(float)*NUM_ITEMS);
						} else { // copy data with adjust index
							// for (int k = 0; k < NUM_ITEMS; k++){
							// 	allGatherResult[whichData[j]*NUM_ITEMS + k] = recvbuf[i][(j - 1)*NUM_ITEMS + k];
							// }
                            memcpy(&allGatherResult[whichData[j]*NUM_ITEMS], &recvbuf[i][(j - 1)*NUM_ITEMS], sizeof(float)*NUM_ITEMS );
						}
					}
					
				}
			}


#if defined(TIME_FOR_EACH_STEP_C)
	// MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		//fprintf(stdout, "%.7lf\tCopy to final result\n", MPI_Wtime() - dblasttimer);
		tstep1copy = MPI_Wtime() - dblasttimer;
		dblasttimer = MPI_Wtime();
	}
#endif
			for (int i = 0; i < d; i++){
				SMPI_SHARED_FREE(recvbuf[i]);
			}
			SMPI_SHARED_FREE (nsendbuf);
			delete (sendbuf);
			delete whichData;
	
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
		fprintf(stdout, "k%d,%.7lf,%d,%.7lf,%.7lf,%.7lf\n", d, kimrdtime, NUM_ITEMS, tstep0, tstep1comm, tstep1copy);
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
	SMPI_SHARED_FREE(data);
	SMPI_SHARED_FREE(allGatherResult);
	MPI_Finalize();
	return 0;
}