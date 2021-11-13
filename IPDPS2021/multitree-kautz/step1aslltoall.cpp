#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
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
using namespace std;

int main(int argc, char *argv[])  {
    int rank;
	int size;
	int hostname_len;
	int NUM_ITEMS = 8195;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int d = (1 + sqrt(-1 + 4*size))/2;
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
    float **recvbuf1 = (float**)malloc(sizeof(float*)*(d));
    for (int i = 0; i < d; i++){
        recvbuf1[i] = (float*)malloc(sizeof(float)*(d + 1)*NUM_ITEMS);
        for (int j = 0; j < (d + 1)*NUM_ITEMS; j++){
            recvbuf1[i][j] = rank*10 + i;
        }
    }
   
    // allocate new size for send buf
    // allocate new size for send buf
    float *sendbuf2 = (float*)malloc(sizeof(float)*NUM_ITEMS*(d + 3));
    float **recvbufv2 = (float**)malloc(sizeof(float*)*(d));
    for (int i = 0; i < d; i++){
        recvbufv2[i] = (float*)malloc(sizeof(float)*(d)*NUM_ITEMS);
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
            // if (rank == 3){
            //     cout << "From rank " << rank << " sendbuf " << i << " ";
            //     for (int j = 0; j < d*NUM_ITEMS; j++){
            //         cout << sendbuf2[j] << " ";
            //     }
            //     cout << endl;
            // }
            
            // cout << "From rank: " << rank << " send to: " << destination << " numitems: " << d << endl;
            MPI_Isend(sendbuf2, NUM_ITEMS*d, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends1[i]);
            // send
            
        } else { // send d - 1 chunks of data
            // prepare data
            for (int j = 0; j < d - 1; j++){
                memcpy(&sendbuf2[j*NUM_ITEMS], &recvbuf1[j + 1][i], NUM_ITEMS*sizeof(float));
            }
            if (rank == 3){
                cerr << "From rank " << rank << " sendbuf " << i << " ";
                for (int j = 0; j < (d - 1)*NUM_ITEMS; j++){
                    cerr << sendbuf2[j] << " ";
                }
                cerr << endl;
            }
            
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
    if (rank == 0){
        cout << "From rank " << rank << " Recvbufv2: " << endl;
        for (int i = 0; i < d; i++){
            cout << "\t";
            for (int j = 0; j < d*NUM_ITEMS; j++){
                cout << recvbufv2[i][j] << " ";
            }
            cout << endl;
        }
    }
    MPI_Finalize();
}