/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:33:06
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-11-14 00:25:06
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
	int NUM_ITEMS = 1011;
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
	if ((data = new float[NUM_ITEMS*size]) == NULL) {
		program_abort("Allocate fail!");
	}

	// All rank fill the buffer with random data
	srandom(RAND_SEED + rank);
	for (int i = 0; i < size; i++) {
        for (int j = 0; j < NUM_ITEMS; j++){
            //data[i] = (float)((rank*1000 + i));
		    data[i*NUM_ITEMS + j] = rank*10 + i; //(float)(1 + 1.0 * (random() % 9));
        }
	}                                                                                                        

#if defined(DEBUG1)
	printf("Data from rank %d: ", rank);
	for (int i = 0; i < NUM_ITEMS*size; i++){
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
	float *result = new float[NUM_ITEMS*size]();
	// copy local data to result
	memcpy(&result[rank*NUM_ITEMS], &data[rank*NUM_ITEMS], NUM_ITEMS*sizeof(float));
	
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		start_time = MPI_Wtime();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLTOALL : START	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	switch(algo) {
		case MULTITREE  :{ 
			// Step 0 send all the neccessary information to all neighbors
			float *sendbuf = (float*)malloc(sizeof(float)*(d + 1)*NUM_ITEMS);
			float **recvbuf = (float**)malloc(sizeof(float*)*d);
			for (int i = 0; i < d; i++){
				recvbuf[i] = (float*)malloc(sizeof(float)*(d + 1)*NUM_ITEMS);
			}
			MPI_Request *reqsends = new MPI_Request[d];
			MPI_Request *reqrecvs = new MPI_Request[d];

			// for all child: 
			for (int i = 0; i < d; i++){
				int aChild = childParent[rank][i];
				int aParent = childParent[rank][d + i];
				int duplicateIdx = childParent[aChild][0];
				//receive data from parents
				if (aParent != childParent[rank][0]){
					MPI_Irecv(recvbuf[i], (d + 1)*NUM_ITEMS, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
				} else {
					MPI_Irecv(recvbuf[i], d*NUM_ITEMS, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
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
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf[i][d*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				} else { // copy with adjust index
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf[i][(d - 1)*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				}
			}
			// cout << "From rank: " << rank << " result after the 1st step " << ": ";
			// for (int j = 0; j < size*NUM_ITEMS; j++){
			// 	cout << result[j] << " ";
			// }
			// cout << endl;
			// cout << "From rank: " << rank << " recvbuf " << ": " << endl;
			// for (int i = 0; i < d ;i ++){
			// 	cout << "\t";
			// 	for (int j = 0; j < (d + 1)*NUM_ITEMS; j++){
			// 		cout << recvbuf[i][j] << " ";
			// 	}
			// 	cout << endl;
			// }
			free(sendbuf);
			delete reqrecvs;
			delete reqsends;

			// Step 1 to d: send all the received data to children, finish the algorithm.

			for (int i = 0; i < d; i ++){
				// Prepare meta data
				int duplicateIdx = childParent[rank][0];
				int indexOfSendData = childParent[rank][d + i];

				reqsends = new MPI_Request[d];
				reqrecvs = new MPI_Request[d];	
				// Receive data from parent
				// for parent i
				if (indexOfSendData != duplicateIdx){ // receive d chunk of data
					// for each parent of parent (j)
					for (int j = 0; j < d; j++){
						int source = childParent[rank][d + j]; 
						int recvIdx = childParent[source][2*d - i];
						// cout << "\tFrom rank: " << rank << " step: " << i << " wait to receive from " << source << " index " << recvIdx << endl;
						MPI_Irecv(&result[recvIdx*NUM_ITEMS], NUM_ITEMS, MPI_FLOAT, source, recvIdx, MPI_COMM_WORLD, &reqrecvs[j]);					
					}
					
				} else { // receive d - 1 chunk of data
					for (int j = 0; j < d - 1; j++){
						int source = childParent[rank][d + j + 1];
						int recvIdx = childParent[source][0];// receive from parent of parent
						// cout << "\tFrom rank: " << rank << " step: " << i << " wait to receive from " << source << " index " << recvIdx << endl;
						MPI_Irecv(&result[recvIdx*NUM_ITEMS], NUM_ITEMS, MPI_FLOAT, source, recvIdx, MPI_COMM_WORLD, &reqrecvs[j]);					
					}
				}

				// IF ERROR SPLIT SEND AND RECEIVE INTO TWO FOR I LOOP
				
				// Send data to children
				// For each child i 
				if (indexOfSendData != duplicateIdx){ // send d chunks of data
					// send the data from parent j
					for (int j = 0; j < d; j++){
						int destination = childParent[rank][j];
						int sendidx = childParent[rank][2*d - i];
						// cout << "\tFrom rank: " << rank << " step: " << i << " send to rank " << destination << " index " << indexOfSendData << endl;
						MPI_Isend(&recvbuf[d - i][j*NUM_ITEMS], NUM_ITEMS, MPI_FLOAT, destination, sendidx, MPI_COMM_WORLD, &reqsends[j]);
					}
				} else { // send d - 1 chunks of data
					for (int j = 0; j < d - 1; j++){
						int destination = childParent[rank][j + 1];
						// cout << "\tFrom rank: " << rank << " step: " << i << " send to rank " << destination << " index " << indexOfSendData << endl;
						MPI_Isend(&recvbuf[i][j*NUM_ITEMS], NUM_ITEMS, MPI_FLOAT, destination, indexOfSendData, MPI_COMM_WORLD, &reqsends[j]);
					}
				}

				if (indexOfSendData != duplicateIdx){ // send d chunks of data
					// send the data from parent j
					for (int j = 0; j < d; j++){
						MPI_Wait(&reqrecvs[j], MPI_STATUS_IGNORE);
					}
				} else { // send d - 1 chunks of data
					for (int j = 0; j < d - 1; j++){
						MPI_Wait(&reqrecvs[j], MPI_STATUS_IGNORE);
					}
				}

				delete reqrecvs;
				delete reqsends;
			}
			

			// cout << "From rank: " << rank << " result after the 2nd step " << ": ";
			// for (int j = 0; j < size*NUM_ITEMS; j++){
			// 	cout << result[j] << " ";
			// }
			// cout << endl;
			break; //optional
		
		} case CONGESTION: {
			// Step 0 send all the neccessary information to all neighbors
			float *sendbuf = (float*)malloc(sizeof(float)*(d + 1)*NUM_ITEMS);
			float **recvbuf = (float**)malloc(sizeof(float*)*d);
			for (int i = 0; i < d; i++){
				recvbuf[i] = (float*)malloc(sizeof(float)*(d + 1)*NUM_ITEMS);
			}
			MPI_Request *reqsends = new MPI_Request[d];
			MPI_Request *reqrecvs = new MPI_Request[d];

			// for all child: 
			for (int i = 0; i < d; i++){
				int aChild = childParent[rank][i];
				int aParent = childParent[rank][d + i];
				int duplicateIdx = childParent[aChild][0];
				//receive data from parents
				if (aParent != childParent[rank][0]){
					MPI_Irecv(recvbuf[i], (d + 1)*NUM_ITEMS, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
				} else {
					MPI_Irecv(recvbuf[i], d*NUM_ITEMS, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
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
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf[i][d*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				} else { // copy with adjust index
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf[i][(d - 1)*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				}
			}
			for (int i =0; i < d; i++){
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			// cout << "From rank: " << rank << " result after the 1st step " << ": ";
			// for (int j = 0; j < size*NUM_ITEMS; j++){
			// 	cout << result[j] << " ";
			// }
			// cout << endl;
			// cout << "From rank: " << rank << " recvbuf " << ": " << endl;
			// for (int i = 0; i < d ;i ++){
			// 	cout << "\t";
			// 	for (int j = 0; j < (d + 1)*NUM_ITEMS; j++){
			// 		cout << recvbuf[i][j] << " ";
			// 	}
			// 	cout << endl;
			// }
			free(sendbuf);
			delete reqsends;
			delete reqrecvs;

			// Step 1: send all the received data to children, finish the algorithm.
			reqsends = new MPI_Request[d*d];
			reqrecvs = new MPI_Request[d*d];
			sendbuf = (float*)malloc(sizeof(float)*NUM_ITEMS);
			for (int i = 0; i < d; i ++){
				// Prepare meta data
				int duplicateIdx = childParent[rank][0];
				int indexOfSendData = childParent[rank][d + i];
				// Receive data from parent
				// for parent i
				if (indexOfSendData != duplicateIdx){ // receive d chunk of data
					// for each parent of parent (j)
					for (int j = 0; j < d; j++){
						int source = childParent[rank][d + i]; // from one source but different data
						int recvIdx = childParent[source][d + j];// receive from parent of parent
						// cout << "\tFrom rank: " << rank << " wait to receive from " << source << " index " << recvIdx << endl;
						MPI_Irecv(&result[recvIdx*NUM_ITEMS], NUM_ITEMS, MPI_FLOAT, source, recvIdx, MPI_COMM_WORLD, &reqrecvs[i*d + j]);					
					}
					
				} else { // receive d - 1 chunk of data
					for (int j = 0; j < d - 1; j++){
						int source = childParent[rank][d + i]; // from one source but different data
						int recvIdx = childParent[source][d + j + 1];// receive from parent of parent
						// cout << "\tFrom rank: " << rank << " wait to receive from " << source << " index " << recvIdx << endl;
						MPI_Irecv(&result[recvIdx*NUM_ITEMS], NUM_ITEMS, MPI_FLOAT, source, recvIdx, MPI_COMM_WORLD, &reqrecvs[i*d + j]);					
					}
				}

				// IF ERROR SPLIT SEND AND RECEIVE INTO TWO FOR I LOOP
				
				// Send data to children
				// For each child i 
				if (indexOfSendData != duplicateIdx){ // send d chunks of data
					// send the data from parent j
					for (int j = 0; j < d; j++){
						int destination = childParent[rank][j];
						// cout << "\tFrom rank: " << rank << " send to rank " << destination << " index " << indexOfSendData << endl;
						MPI_Isend(&recvbuf[i][j*NUM_ITEMS], NUM_ITEMS, MPI_FLOAT, destination, indexOfSendData, MPI_COMM_WORLD, &reqsends[i*d + j]);
					}
				} else { // send d - 1 chunks of data
					for (int j = 0; j < d - 1; j++){
						int destination = childParent[rank][j + 1];
						// cout << "\tFrom rank: " << rank << " send to rank " << destination << " index " << indexOfSendData << endl;
						MPI_Isend(&recvbuf[i][j*NUM_ITEMS], NUM_ITEMS, MPI_FLOAT, destination, indexOfSendData, MPI_COMM_WORLD, &reqsends[i*d + j]);
					}
				}
			}
			for (int i = 0; i < d; i ++){
				int duplicateIdx = childParent[rank][0];
				int waitIdx = childParent[rank][d + i];
				if (waitIdx != duplicateIdx){ // send d chunks of data
					// send the data from parent j
					for (int j = 0; j < d; j++){
						MPI_Wait(&reqrecvs[i*d + j], MPI_STATUS_IGNORE);
					}
				} else { // send d - 1 chunks of data
					for (int j = 0; j < d - 1; j++){
						MPI_Wait(&reqrecvs[i*d + j], MPI_STATUS_IGNORE);
					}
				}
			}

			// cout << "From rank: " << rank << " result after the 2nd step " << ": ";
			// for (int j = 0; j < size*NUM_ITEMS; j++){
			// 	cout << result[j] << " ";
			// }
			// cout << endl;
			delete reqsends;
			delete reqrecvs;
			break; //optional
		} case COMBINE:{
			// Step 0 send all the neccessary information to all neighbors

			MPI_Request *reqsends = new MPI_Request[d];
			MPI_Request *reqrecvs = new MPI_Request[d];
			float *sendbuf1 = (float*)malloc(sizeof(float)*(d + 1)*NUM_ITEMS);
			float **recvbuf1 = (float**)malloc(sizeof(float*)*d);
			
			for (int i = 0; i < d; i++){
				recvbuf1[i] = (float*)malloc(sizeof(float)*(d + 1)*NUM_ITEMS);
			}
			
			
			// for all child: 
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
					MPI_Irecv(recvbuf1[i], (d + 1)*NUM_ITEMS, MPI_FLOAT, aParent, tag, MPI_COMM_WORLD, &reqrecvs[i]);
				} else {
					// cout << "From rank: " << rank << " wait to receive from: " << aParent << " tag "<< 1 << endl;
					MPI_Irecv(recvbuf1[i], d*NUM_ITEMS, MPI_FLOAT, aParent, 1, MPI_COMM_WORLD, &reqrecvs[i]);
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

			for (int i = 0; i < d; i++){
				// Copy data to final result
				int duplicateIdx = childParent[rank][0];
				int cpyIdx = childParent[rank][d + i];
				if(cpyIdx != duplicateIdx){ //normal copy
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf1[i][d*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				} else { // copy with adjust index
					memcpy(&result[cpyIdx*NUM_ITEMS], &recvbuf1[i][(d - 1)*NUM_ITEMS], NUM_ITEMS*sizeof(float));
				}
			}
            int value = 0;
            if (rank == 0) {
                MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );

                
            }
            else {
                MPI_Recv( &value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (rank < size - 1) 
                MPI_Send( &value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD );

                // cout << "From rank: " << rank << " result after the 1st step " << ": ";
                // for (int j = 0; j < size*NUM_ITEMS; j++){
                //     cout << result[j] << " ";
                // }
                // cout << endl;
                // cout << "From rank: " << rank << " recvbuf " << ": " << endl;
                // for (int i = 0; i < d ;i ++){
                //     cout << "\t";
                //     for (int j = 0; j < (d + 1)*NUM_ITEMS; j++){
                //         cout << recvbuf1[i][j] << " ";
                //     }
                //     cout << endl;
                // }
            }
            
                cout << "From rank: " << rank << " result after the 1st step " << ": ";
                for (int j = 0; j < size*NUM_ITEMS; j++){
                    cout << result[j] << " ";
                }
                cout << endl;
                cout << "From rank: " << rank << " recvbuf " << ": " << endl;
                for (int i = 0; i < d ;i ++){
                    cout << "\t";
                    for (int j = 0; j < (d + 1)*NUM_ITEMS; j++){
                        cout << recvbuf1[i][j] << " ";
                    }
                    cout << endl;
                }
			
			
			free(sendbuf1);
			delete reqrecvs;
			delete reqsends;
			
			// if (rank == 0){
			// 	cout << "1\n";
			// 	cin.get();
			// }
			
			
			//////////////////////////////////////////////////////////////////////////////////////////
			// Step 1 combine the message and send to the children

			// allocate new size for send buf
			float *sendbuf2 = (float*)malloc(sizeof(float)*NUM_ITEMS*(d + 3));
			float **recvbufv2 = (float**)malloc(sizeof(float*)*(d + 3));
			for (int i = 0; i < d; i++){
				recvbufv2[i] = (float*)malloc(sizeof(float)*(d + 3)*NUM_ITEMS);
			}
			MPI_Request *reqsends1 = new MPI_Request[d];
			MPI_Request *reqrecvs1 = new MPI_Request[d];

			
			for (int i = 0; i < d; i ++){
				// Prepare meta data
				int duplicateIdx = childParent[rank][0];

				int source = childParent[rank][d + i];
				if (source != duplicateIdx){
					// cout << "From rank: " << rank << " wait to recieve from: " << source << " numitems: " << d << endl;
					MPI_Irecv(recvbufv2[i], d*NUM_ITEMS, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs1[i]);
				} else {
					// if (rank ==0 ){
					//     cout << "From rank: " << rank << " wait to recieve from: " << source << " store position: " << i << endl;
					// }
					
					MPI_Irecv(recvbufv2[i], (d - 1)*NUM_ITEMS, MPI_FLOAT, source, 100, MPI_COMM_WORLD, &reqrecvs1[i]);
				}
			}
			for (int i = 0; i < d; i ++){
				int duplicateIdx = childParent[rank][0];
				// Send data to children
				// For each child i 
				int destination = childParent[rank][i];
				if (destination != duplicateIdx){ // send d chunks of data

					// prepare data
					memcpy(&sendbuf2[0], &recvbuf1[0][(i - 1)*NUM_ITEMS], NUM_ITEMS*sizeof(float));
					for (int j = 1; j < d; j++){
						memcpy(&sendbuf2[j*NUM_ITEMS], &recvbuf1[j][i*NUM_ITEMS], NUM_ITEMS*sizeof(float));
					}

                    cout << "From rank " << rank << " sendbuf " << i << " ";
                    for (int j = 0; j < d*NUM_ITEMS; j++){
                        cout << sendbuf2[j] << " ";
                    }
                    cout << endl;
					
					// cout << "From rank: " << rank << " send to: " << destination << " numitems: " << d << endl;
					MPI_Isend(sendbuf2, NUM_ITEMS*d, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends1[i]);
					// send
					
				} else { // send d - 1 chunks of data
					// prepare data
					for (int j = 0; j < d - 1; j++){
						memcpy(&sendbuf2[j*NUM_ITEMS], &recvbuf1[j + 1][i], NUM_ITEMS*sizeof(float));
					}

                    
                    cout << "From rank " << rank << " sendbuf " << i << " ";
                    for (int j = 0; j < (d - 1)*NUM_ITEMS; j++){
                        cout << sendbuf2[j] << " ";
                    }
                    cout << endl;
					
					// send 
					// cout << "From rank: " << rank << " send to: " << destination << " numitems: " << d - 1  << endl;
					MPI_Isend(sendbuf2, (d - 1)*NUM_ITEMS, MPI_FLOAT, destination, 100, MPI_COMM_WORLD, &reqsends1[i]);

				}
			}
			for (int i = 0; i < d; i++){
				MPI_Wait(&reqsends1[i], MPI_STATUS_IGNORE);
				
			}

			for (int i = 0; i < d; i++){
				// Wait for recieving complete
				MPI_Wait(&reqrecvs1[i], MPI_STATUS_IGNORE);
			}   
            
            cout << "From rank " << rank << " Recvbufv2: " << endl;
            for (int i = 0; i < d; i++){
                cout << "\t";
                for (int j = 0; j < d*NUM_ITEMS; j++){
                    cout << recvbufv2[i][j] << " ";
                }
                cout << endl;
            }
			
			int parent, idx;
			for (int i = 0; i < d; i++){
				// Copy to the final result
				if(i == 0){ // duplicate index
					// For each data chunk in the received buf
					
					for (int j = 0; j < d - 1; j++){
						// calculate index of the data
						parent = childParent[rank][d + i];
						idx = childParent[parent][d + j + 1];
						// cout << "From rank: " << rank << " write data of " << parent << " to idx: " << idx << endl;
						// copy to the final result
						memcpy(&result[idx*NUM_ITEMS], &recvbufv2[i][j*NUM_ITEMS], NUM_ITEMS*sizeof(float));			
					}
				} else {
					for (int j = 0; j < d; j++){
						// calculate index of the data
						parent = childParent[rank][d + i];
						idx = childParent[parent][d + j];
						// cout << "From rank: " << rank << " write data of " << parent << " to idx: " << idx << endl;
						// copy to the final result
						memcpy(&result[idx*NUM_ITEMS], &recvbufv2[i][j*NUM_ITEMS], NUM_ITEMS*sizeof(float));
					}
				}
			}

            cout << "From rank: " << rank << " final result " << ": ";
            for (int j = 0; j < size*NUM_ITEMS; j++){
                cout << result[j] << " ";
            }
            cout << endl;


			delete reqrecvs1;
			delete reqsends1;
			free(sendbuf2);
			for (int i = 1; i < d; i++){
				free(recvbuf1[i]);
				free(recvbufv2[i]);
			}
			free(recvbufv2);
			free(recvbuf1);
		} default : //Optional
			break;											
	}
	

	
#if defined(PRINT_RESULT)
	// Print allgather result after step 
	if (rank == 0){
		cout << "From rank " << rank << " allgatherresult:\n\t";
		for (int i = 0; i < NUM_ITEMS*size; i++){
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
		fprintf(stdout, "k%d,%.7lf,%d\n", d, kimrdtime, NUM_ITEMS);
	}
#if defined(COMPARE_BUILDIN)
	start_time = MPI_Wtime();
	float *resultlib = (float*)malloc(sizeof(float)*NUM_ITEMS*size);
	MPI_Alltoall(data, NUM_ITEMS, MPI_FLOAT, resultlib, NUM_ITEMS, MPI_FLOAT, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if ((0 == rank)) {
		fprintf(stdout, "k%d,%.7lf,%.7lf,%d\n", d, kimrdtime, MPI_Wtime() - start_time, NUM_ITEMS);
	}

	// Compare the result
	if (rank == 0){
		for (int i = 0; i < NUM_ITEMS*size; i++){
			if ((result[i]) != (resultlib[i])){
				cout << "Index " << i << " kim: " << result[i] << " lib " << resultlib[i] << endl;
				fprintf(stdout, "%s\n", "Alltoall wrong");
				break;
			}
		}
	}
#endif
	delete data;
	delete result;
	MPI_Finalize();
	
	return 0;
}