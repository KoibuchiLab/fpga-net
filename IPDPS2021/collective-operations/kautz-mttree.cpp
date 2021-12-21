/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:33:06
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-12-22 02:31:04
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

extern "C" {
	// Get declaration for f(int i, char c, float x)
#include "kmpi.h"
}

using namespace std;
#define RAND_SEED 721311
struct Int3 {
	int dst;
	int src;
	int sendidx;
	int recvidx;
};

int h2r(const char* hostname, int degree);
int hidx2r(const int a, const int b, int degree);
void r2h(int rank, int degree, int& a, int& b);
void r2h_r(int rank, int degree, char* hostname);
void program_abort(const char* message);
using namespace std;

int KMPI_Allgatherf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
	float* recvbuf, int recvcount, MPI_Datatype recvtype,
	MPI_Comm comm){
	int rank;
	int size;
	int hostname_len;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Calculate degree
	int d = (1 + sqrt(-1 + 4*size))/2;
	
	char hostname[256];
	MPI_Get_processor_name(hostname, &hostname_len);
	char topo[256];
	// Topology optional argument

#if defined(DEBUG0)
	printf("Hostname: %s | Rank: %d\n", hostname, rank);
#endif

	if (size != d*(d+1)){
		program_abort("Number of process must equal to # node Kautz graph diameter 2\n");
	}
	
	// Start the timer
	double start_time;
	
	int groupnumber, nodenumber;
	r2h(rank, d, groupnumber, nodenumber);
	if (rank != h2r(hostname, d))
		program_abort("Computed rank must equal to real rank\n");

	int numofgroups = size/d;
	int numofnodesingroup = d;

	vector <Int3>scheduleTable;
	ifstream file;
	std::string filename = "scheduleTable/kout" + to_string(d);
	file.open(filename);
	// reduce memory used: each process hold one line of this table
	string line;


	for (int i = 0; i < size; i++) {
		getline(file, line);
		if (i == rank) {
			istringstream iss(line);
			string schedule;
			while (iss >> schedule) {
				Int3 tmp;
				sscanf(schedule.c_str(), "%d,%d,%d,%d", &(tmp.dst), &tmp.src, &(tmp.sendidx), &tmp.recvidx);
				scheduleTable.push_back(tmp);
			}
		}
	}

	// copy local data to result
	for (int i = 0; i < sendcount; i++){
		recvbuf[rank*sendcount + i] = sendbuf[i];
	}
	MPI_Request *reqrecvs;
	MPI_Request *reqsends;
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		start_time = MPI_Wtime();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	  ALLGATHER : START	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int numOfStep = d + 1;

	for (int step = 0; step < numOfStep; step++) {
		if (step == 0) { // Step == 0 send to d nodes
			// Read schedule table first d collumn, process rank i read line i

			// a cell in schetable contain dst destination, dtaIndex place of data
			// See scheduletable.txt

			reqrecvs = new MPI_Request[d];
			reqsends = new MPI_Request[d];

			for (int i = 0; i < d; i++) {
				int destination = scheduleTable[i].dst;
				int source = scheduleTable[i].src;
				int sendIdx = scheduleTable[i].sendidx * sendcount;
				int recvIdx = scheduleTable[i].src * recvcount;
				//cout << "From rank " << rank << " destination: " << destination \
								<< " data index: " << sendIdx << endl;
				MPI_Irecv(&recvbuf[recvIdx], recvcount, MPI_FLOAT, source, \
					0, MPI_COMM_WORLD, &reqrecvs[i]);
				MPI_Isend(&recvbuf[sendIdx], recvcount, MPI_FLOAT, destination, \
					0, MPI_COMM_WORLD, &reqsends[i]);
			}

			for (int i = 0; i < d; i++) {
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			for (int i = 0; i < d; i++) {
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			delete reqrecvs;
			delete reqsends;
		} else if (step == 1) { //step = 1, then send to d - 1 node
			reqrecvs = new MPI_Request[d - 1];
			reqsends = new MPI_Request[d - 1];

			for (int i = 0; i < d - 1; i++) {
				int source = scheduleTable[d + i].src;
				int destination = scheduleTable[d + i].dst;
				int sendIdx = scheduleTable[d + i].sendidx * sendcount;
				int recvIdx = scheduleTable[d + i].recvidx * recvcount;
				//printf("->> From rank %d, source: %d, dest: %d, sendidx: %d, recvidx: %d\n",\
								rank, source, destination, sendIdx, recvIdx);
				MPI_Irecv(&recvbuf[recvIdx], recvcount, MPI_FLOAT,
					source, 0, MPI_COMM_WORLD, &reqsends[i]);
				MPI_Isend(&recvbuf[sendIdx], sendcount, MPI_FLOAT, \
					destination, 0, MPI_COMM_WORLD, &reqrecvs[i]);
			}

			for (int i = 0; i < d - 1; i++) {
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			for (int i = 0; i < d - 1; i++) {
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			delete reqsends;
			delete reqrecvs;
		} else { // send to d nodes with different index
			reqrecvs = new MPI_Request[d];
			reqsends = new MPI_Request[d];

			for (int i = 0; i < d; i++) {
				int source = scheduleTable[step * d + i - 1].src;
				int destination = scheduleTable[step * d + i - 1].dst;
				int sendIdx = scheduleTable[step * d + i - 1].sendidx * sendcount;
				int recvIdx = scheduleTable[step * d + i - 1].recvidx * recvcount;

				MPI_Irecv(&recvbuf[recvIdx], recvcount, MPI_FLOAT, source, \
					0, MPI_COMM_WORLD, &reqrecvs[i]);
				MPI_Isend(&recvbuf[sendIdx], sendcount, MPI_FLOAT, destination, \
					0, MPI_COMM_WORLD, &reqsends[i]);
		}

			for (int i = 0; i < d; i++) {
				MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
			}
			for (int i = 0; i < d; i++) {
				MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
			}
			delete reqrecvs;
			delete reqsends;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////	 ALLGATHER : END	     ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// Stop timer
	return MPI_SUCCESS;
}

int KMPI_Alltoallf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
	float* recvbuf, int recvcount, MPI_Datatype recvtype,
	MPI_Comm comm){
	int rank;
	int size;
	int hostname_len;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Calculate degree
	int d = (1 + sqrt(-1 + 4 * size)) / 2;
	char hostname[256];
	MPI_Get_processor_name(hostname, &hostname_len);
	char topo[256];

#if defined(DEBUG0)
	printf("Hostname: %s | Rank: %d\n", hostname, rank);
#endif

	if (size != d * (d + 1)) {
		program_abort("Number of process must equal to # node Kautz graph diameter 2\n");
	}

	int groupnumber, nodenumber;
	r2h(rank, d, groupnumber, nodenumber);
	if (rank != h2r(hostname, d))
		program_abort("Computed rank must equal to real rank\n");

	int numofgroups = size / d;
	int numofnodesingroup = d;

	vector <int>* childParent;
	// Open file
	ifstream file;
	string line;
	std::string filename = "scheduleTable/kcmb" + to_string(d);
	file.open(filename);
	int tmp;
	childParent = new vector <int>[size];
	for (int i = 0; i < size; i++) {
		getline(file, line);
		istringstream iss(line);
		string alist;
		for (int j = 0; j < 2 * d + 1; j++) {
			iss >> tmp;
			childParent[i].push_back(tmp);
		}
	}
	file.close();

	memcpy(&recvbuf[rank * recvcount], &sendbuf[rank * recvcount], recvcount * sizeof(float));

	float** sendbuf0 = (float**)malloc(sizeof(float*) * d);
	float** recvbuf0 = (float**)malloc(sizeof(float*) * d);
	for (int i = 0; i < d; i++) {
		recvbuf0[i] = (float*)malloc(sizeof(float) * (d + 1) * recvcount);
		sendbuf0[i] = (float*)malloc(sizeof(float) * (d + 1) * sendcount);
	}
	MPI_Request* reqsends = new MPI_Request[d];
	MPI_Request* reqrecvs = new MPI_Request[d];

	// for all child: 
	for (int i = 0; i < d; i++) {
		int aChild = childParent[rank][i];
		int aParent = childParent[rank][d + i];
		int duplicateIdx = childParent[aChild][0];
		//receive data from parents
		if (aParent != childParent[rank][0]) {
			MPI_Irecv(recvbuf0[i], (d + 1) * recvcount, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
		}
		else {
			MPI_Irecv(recvbuf0[i], d * recvcount, MPI_FLOAT, aParent, 0, MPI_COMM_WORLD, &reqrecvs[i]);
		}

		// Prepare sendbuffer
		// copy data of child of child to sendbuf
		if (rank != duplicateIdx) { // normal copy
			for (int j = 0; j < d; j++) {
				memcpy(&sendbuf0[i][j * sendcount], &sendbuf[childParent[aChild][j] * sendcount], sendcount * sizeof(float));
			}
			memcpy(&sendbuf0[i][d * sendcount], &sendbuf[aChild * sendcount], sendcount * sizeof(float));

			MPI_Isend(sendbuf0[i], (d + 1) * sendcount, MPI_FLOAT, aChild, 0, MPI_COMM_WORLD, &reqsends[i]);
		}
		else { // copy with adjust index
			for (int j = 0; j < d - 1; j++) {
				memcpy(&sendbuf0[i][j * sendcount], &sendbuf[childParent[aChild][j + 1] * sendcount], sendcount * sizeof(float));
			}
			memcpy(&sendbuf0[i][(d - 1) * sendcount], &sendbuf[aChild * sendcount], sendcount * sizeof(float));

			MPI_Isend(sendbuf0[i], d * sendcount, MPI_FLOAT, aChild, 0, MPI_COMM_WORLD, &reqsends[i]);
		}

	}

	for (int i = 0; i < d; i++) {
		MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
		// Copy data to final result
		int duplicateIdx = childParent[rank][0];
		int cpyIdx = childParent[rank][d + i];
		if (cpyIdx != duplicateIdx) { //normal copy
			memcpy(&recvbuf[cpyIdx * recvcount], &recvbuf0[i][d * recvcount], recvcount * sizeof(float));
		}
		else { // copy with adjust index
			memcpy(&recvbuf[cpyIdx * recvcount], &recvbuf0[i][(d - 1) * recvcount], recvcount * sizeof(float));
		}
	}

	for (int i = 0; i < d; i++){
		free(sendbuf0[i]);
	}
	free(sendbuf0);
	delete reqrecvs;
	delete reqsends;

	// Step 1 to d: send all the received data to children, finish the algorithm.

	for (int i = 0; i < d; i++) {
		// Prepare meta data
		int duplicateIdx = childParent[rank][0];
		int indexOfSendData = childParent[rank][d + i];

		reqsends = new MPI_Request[d];
		reqrecvs = new MPI_Request[d];
		// Receive data from parent
		// for parent i
		if (indexOfSendData != duplicateIdx) { // receive d chunk of data
			// for each parent of parent (j)
			for (int j = 0; j < d; j++) {
				int source = childParent[rank][d + j];
				int recvIdx = childParent[source][2 * d - i];
				// cout << "\tFrom rank: " << rank << " step: " << i << " wait to receive from " << source << " index " << recvIdx << endl;
				MPI_Irecv(&recvbuf[recvIdx * recvcount], recvcount, MPI_FLOAT, source, recvIdx, MPI_COMM_WORLD, &reqrecvs[j]);
			}

		}
		else { // receive d - 1 chunk of data
			for (int j = 0; j < d - 1; j++) {
				int source = childParent[rank][d + j + 1];
				int recvIdx = childParent[source][0];// receive from parent of parent
				// cout << "\tFrom rank: " << rank << " step: " << i << " wait to receive from " << source << " index " << recvIdx << endl;
				MPI_Irecv(&recvbuf[recvIdx * recvcount], recvcount, MPI_FLOAT, source, recvIdx, MPI_COMM_WORLD, &reqrecvs[j]);
			}
		}

		// IF ERROR SPLIT SEND AND RECEIVE INTO TWO FOR I LOOP

		// Send data to children
		// For each child i 
		if (indexOfSendData != duplicateIdx) { // send d chunks of data
			// send the data from parent j
			for (int j = 0; j < d; j++) {
				int destination = childParent[rank][j];
				int sendidx = childParent[rank][2 * d - i];
				// cout << "\tFrom rank: " << rank << " step: " << i << " send to rank " << destination << " index " << indexOfSendData << endl;
				MPI_Isend(&recvbuf0[d - i][j * recvcount], recvcount, MPI_FLOAT, destination, sendidx, MPI_COMM_WORLD, &reqsends[j]);
			}
		} else { // send d - 1 chunks of data
			for (int j = 0; j < d - 1; j++) {
				int destination = childParent[rank][j + 1];
				// cout << "\tFrom rank: " << rank << " step: " << i << " send to rank " << destination << " index " << indexOfSendData << endl;
				MPI_Isend(&recvbuf0[i][j * recvcount], recvcount, MPI_FLOAT, destination, indexOfSendData, MPI_COMM_WORLD, &reqsends[j]);
			}
		}

		if (indexOfSendData != duplicateIdx) { // send d chunks of data
			// send the data from parent j
			for (int j = 0; j < d; j++) {
				MPI_Wait(&reqrecvs[j], MPI_STATUS_IGNORE);
			}
		} else { // send d - 1 chunks of data
			for (int j = 0; j < d - 1; j++) {
				MPI_Wait(&reqrecvs[j], MPI_STATUS_IGNORE);
			}
		}
		
		
		delete reqrecvs;
		delete reqsends;
		
	}
	for (int i = 0; i < d; i++) {
		free(recvbuf0[i]);
	}
	free(recvbuf0);
	return (MPI_SUCCESS);
}

int h2r(const char* hostname, int degree) {
	int a, b;
	sscanf(hostname, "%d,%d", &a, &b);
	if (b > a) {
		return degree * a + b - 1;
	}
	else {
		return degree * a + b;
	}
}

int hidx2r(const int a, const int b, int degree) {
	if (b > a) {
		return degree * a + b - 1;
	}
	else {
		return degree * a + b;
	}
}

void r2h(int rank, int degree, int& groupnumber, int& nodenumber) {
	groupnumber = rank / degree;
	nodenumber = rank % degree;
	if (nodenumber >= groupnumber) nodenumber++;
	return;
}

void r2h_r(int rank, int degree, char* hostname) {
	int groupnumber = rank / degree;
	int nodenumber = rank % degree;

	if (nodenumber >= groupnumber) nodenumber++;
	sprintf(hostname, "%d,%d", groupnumber, nodenumber);
	return;
}

void program_abort(const char* message) {
	fprintf(stderr, "Error: %s", message);
	exit(1);
}

