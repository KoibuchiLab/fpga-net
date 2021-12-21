#include "kmpi.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <list>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cmath>
#include <string>
#include <mpi.h>
#include <stdio.h>

using namespace std;
struct Int3 {
    int dst;
    int src;
    int sendidx;
    int recvidx;
};

#define MULTITREE	0
#define CONGESTION	1
#define COMBINE		2
int h2r(const char* hostname, int degree);
int hidx2r(const int a, const int b, int degree);
void r2h(int rank, int degree, int& a, int& b);
void r2h_r(int rank, int degree, char* hostname);
void program_abort(const char* message);


int KMPI_Allgatherf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
    float* recvbuf, int recvcount, MPI_Datatype recvtype,
    MPI_Comm comm){
    int rank;
    int size;
    int hostname_len;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
    double dblasttimer;
    // Calculate degree
    int d = (1 + sqrt(-1 + 4 * size)) / 2;

    char hostname[256];
    MPI_Get_processor_name(hostname, &hostname_len);
    

#if defined(DEBUG0)
    printf("Hostname: %s | Rank: %d\n", hostname, rank);
#endif

    if (size != d * (d + 1)) {
        printf("Number of process must equal to # node Kautz graph diameter 2\n");
    }

    // Allocate memory for data
    float* data;
    if ((data = new float[sendcount]) == NULL) {
        printf("Out of memory!");
    }


    // Start the timer
    double start_time;

    int groupnumber, nodenumber;
    r2h(rank, d, groupnumber, nodenumber);
    if (rank != h2r(hostname, d))
        program_abort("Computed rank must equal to real rank\n");

    int numofgroups = size / d;
    int numofnodesingroup = d;

    vector <Int3>scheduleTable;
    vector <int>* childParent;
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
    

    // allocate memory for result
    float* allGatherResult = new float[sendcount * size]();
    // copy local data to result
    float** recvbuf0;
    float* sendbuf0;
    recvbuf0 = new float* [d];
    for (int i = 0; i < d; i++) {
        recvbuf0[i] = new float[d * recvcount];
    }
    sendbuf0 = new float[d * sendcount];

    MPI_Request* reqrecvs;
    MPI_Request* reqsends;

    reqrecvs = new MPI_Request[d];
    reqsends = new MPI_Request[d];
    int source, destination, sendidx, recvidx;
    for (int i = 0; i < d; i++) {
        destination = childParent[rank][i];
        source = childParent[rank][d + i];
        sendidx = rank * sendcount;
        recvidx = childParent[rank][d + i] * recvcount;
        MPI_Irecv(&allGatherResult[recvidx], recvcount, MPI_FLOAT, source, 0, \
            comm, &reqrecvs[i]);
        MPI_Isend(&allGatherResult[sendidx], sendcount, MPI_FLOAT, destination, 0, \
            comm, &reqsends[i]);
    }
    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
    }
    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
    }
    delete reqsends;
    delete reqrecvs;

    // Step 1 send receives informantion to all child node
    reqrecvs = new MPI_Request[d];
    reqsends = new MPI_Request[d];

    int duplicateIdx = childParent[rank][2 * d];

    //prepare sendbuf
    for (int j = 0; j < d; j++) {
        memcpy(&sendbuf0[j * sendcount], &allGatherResult[childParent[rank][d + j] * sendcount], sizeof(float) * sendcount);
    }
    int tmpi;

    // send and receive message
    for (int i = 0; i < d; i++) {
        source = childParent[rank][d + i];
        if (source == duplicateIdx) {
            MPI_Irecv(recvbuf0[i], recvcount * (d - 1), MPI_FLOAT, source, 0, comm, &reqrecvs[i]);
        }
        else {
            MPI_Irecv(recvbuf0[i], recvcount * d, MPI_FLOAT, source, 0, comm, &reqrecvs[i]);
        }

        destination = childParent[rank][i];
        if (destination != duplicateIdx) {
            MPI_Isend(sendbuf0, sendcount * d, MPI_FLOAT, destination, 0, comm, &reqsends[i]);
        }
        else {
            tmpi = i;
        }
    }
    float* nsendbuf = new float[(d - 1) * sendcount];

    // send the message at the duplicate index
    bool detectduplicate = false;
    for (int j = 0; j < d; j++) {
        if (!detectduplicate) {
            if (childParent[rank][d + j] == duplicateIdx) {
                detectduplicate = true;
                continue;
            }
            memcpy(&nsendbuf[j * sendcount], &allGatherResult[childParent[rank][d + j] * sendcount], sizeof(float) * sendcount);
            // for (int k = 0; k < NUM_ITEMS; k++)
            // 	nsendbuf[j*NUM_ITEMS + k] = allGatherResult[childParent[rank][d + j]*NUM_ITEMS + k];
        }
        else {
            // for (int k = 0; k < NUM_ITEMS; k++)
            // 	nsendbuf[(j - 1)*NUM_ITEMS + k] = allGatherResult[childParent[rank][d + j]*NUM_ITEMS + k];
            memcpy(&nsendbuf[(j - 1) * sendcount], &allGatherResult[childParent[rank][d + j] * sendcount], sizeof(float) * sendcount);
        }
    }
    MPI_Isend(nsendbuf, sendcount * (d - 1), MPI_FLOAT, duplicateIdx, 0, comm, &reqsends[tmpi]);


    int* whichData = new int[d];
    // wait for send and reseive complete and copy buffer to final result
    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
    }


    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < d; i++) {
        // copy buffer to final result
        // find the data source

        // copy the source of data from its schedule 
        for (int j = 0; j < d; j++) {
            whichData[j] = childParent[childParent[rank][d + i]][d + j];
        }
        if (childParent[rank][d + i] != duplicateIdx) {
            for (int j = 0; j < d; j++) {
                // for (int k = 0; k < NUM_ITEMS; k++){
                // 	allGatherResult[whichData[j]*NUM_ITEMS + k] = recvbuf[i][j*NUM_ITEMS + k];
                // }
                memcpy(&allGatherResult[whichData[j] * recvcount], &recvbuf0[i][j * recvcount], sizeof(float) * recvcount);
            }
        }
        else {
            detectduplicate = false;
            for (int j = 0; j < d; j++) {
                if (whichData[j] == childParent[duplicateIdx][2 * d]) {
                    detectduplicate = true;
                    continue;
                }

                if (!detectduplicate) {//Normal copy							
                    // for (int k = 0; k < NUM_ITEMS; k++){
                    // 	allGatherResult[whichData[j]*NUM_ITEMS + k] = recvbuf[i][(j)*NUM_ITEMS + k];
                    // }
                    memcpy(&allGatherResult[whichData[j] * recvcount], &recvbuf0[i][(j)*recvcount], sizeof(float) * recvcount);
                }
                else { // copy data with adjust index
                 // for (int k = 0; k < NUM_ITEMS; k++){
                 // 	allGatherResult[whichData[j]*NUM_ITEMS + k] = recvbuf[i][(j - 1)*NUM_ITEMS + k];
                 // }
                    memcpy(&allGatherResult[whichData[j] * recvcount], &recvbuf0[i][(j - 1) * recvcount], sizeof(float) * recvcount);
                }
            }

        }
    }

    for (int i = 0; i < d; i++) {
        delete recvbuf0[i];
    }
    delete nsendbuf;
    delete sendbuf0;
    delete whichData;
    return(MPI_SUCCESS);
}


int KMPI_Alltoallf(const float* sendbuf, int sendcount, MPI_Datatype sendtype,
    float* recvbuf, int recvcount, MPI_Datatype recvtype,
    MPI_Comm comm){

    int rank;
    int size;
    int hostname_len;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
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

    // Allocate memory for data

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

    // copy local data to result
    memcpy(&recvbuf[rank * recvcount], &sendbuf[rank * recvcount], recvcount * sizeof(float));

    
    // Step 0 send all the neccessary information to all neighbors
    MPI_Request* reqsends = new MPI_Request[d];
    MPI_Request* reqrecvs = new MPI_Request[d];
    float** sendbuf1 = (float**)malloc(sizeof(float*) * d);
    float** recvbuf1 = (float**)malloc(sizeof(float*) * d);

    for (int i = 0; i < d; i++) {
        recvbuf1[i] = (float*)malloc(sizeof(float) * (d + 1) * recvcount);
        sendbuf1[i] = (float*)malloc(sizeof(float) * (d + 1) * sendcount);
    }


    // for all child: 
    for (int i = 0; i < d; i++) {
        int aChild = childParent[rank][i];
        int aParent = childParent[rank][d + i];
        int duplicateIdx = childParent[aChild][0];
        //receive data from parents
        int tag = aParent;
        for (int j = 0; j < d; j++) {
            tag += childParent[aParent][d + j];
        }
        if (aParent != childParent[rank][0]) {
            // cout << "From rank: " << rank << " wait to receive from: " << aParent << " tag "<< tag << endl;
            MPI_Irecv(recvbuf1[i], (d + 1) * recvcount, MPI_FLOAT, aParent, tag, comm, &reqrecvs[i]);
        }
        else {
            // cout << "From rank: " << rank << " wait to receive from: " << aParent << " tag "<< 1 << endl;
            MPI_Irecv(recvbuf1[i], d * recvcount, MPI_FLOAT, aParent, 1, comm, &reqrecvs[i]);
        }
    }

    for (int i = 0; i < d; i++) {
        // Prepare sendbuffer
        // copy data of child of child to send buf
        int aChild = childParent[rank][i];
        int duplicateIdx = childParent[aChild][0];
        int tag = rank;
        if (rank != duplicateIdx) { // normal copy
            for (int j = 0; j < d; j++) {
                memcpy(&sendbuf1[i][j * sendcount], &sendbuf[childParent[aChild][j] * sendcount], sendcount * sizeof(float));
                tag += childParent[rank][d + j];
            }
            memcpy(&sendbuf1[i][d * sendcount], &sendbuf[aChild * sendcount], sendcount * sizeof(float));
            // cout << "From rank: " << rank << " send to: " << aChild << " tag "<< tag << endl;
            MPI_Isend(sendbuf1[i], (d + 1) * sendcount, MPI_FLOAT, aChild, tag, comm, &reqsends[i]);
        }
        else { // copy with adjust index
            for (int j = 0; j < d - 1; j++) {
                memcpy(&sendbuf1[i][j * sendcount], &sendbuf[childParent[aChild][j + 1] * sendcount], sendcount * sizeof(float));
            }
            memcpy(&sendbuf1[i][(d - 1) * sendcount], &sendbuf[aChild * sendcount], sendcount * sizeof(float));
            // cout << "From rank: " << rank << " send to: " << aChild << " tag "<< 1 << endl;
            MPI_Isend(sendbuf1[i], d * sendcount, MPI_FLOAT, aChild, 1, comm, &reqsends[i]);
        }

    }

    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
    }
    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < d; i++) {
        // Copy data to final result
        int duplicateIdx = childParent[rank][0];
        int cpyIdx = childParent[rank][d + i];
        if (cpyIdx != duplicateIdx) { //normal copy
            memcpy(&recvbuf[cpyIdx * recvcount], &recvbuf1[i][d * recvcount], recvcount * sizeof(float));
        }
        else { // copy with adjust index
            memcpy(&recvbuf[cpyIdx * recvcount], &recvbuf1[i][(d - 1) * recvcount], recvcount * sizeof(float));
        }
    }

  
    free(sendbuf1);
    delete reqrecvs;
    delete reqsends;

    //////////////////////////////////////////////////////////////////////////////////////////
    // Step 1 combine the message and send to the children

    // allocate new size for send buf
    float** sendbufv2 = (float**)malloc(sizeof(float*) * (d + 3));
    float** recvbufv2 = (float**)malloc(sizeof(float*) * (d + 3));
    for (int i = 0; i < d; i++) {
        recvbufv2[i] = (float*)malloc(sizeof(float) * (d + 3) * recvcount);
        sendbufv2[i] = (float*)malloc(sizeof(float) * (d + 3) * recvcount);
    }
    MPI_Request* reqsends1 = new MPI_Request[d];
    MPI_Request* reqrecvs1 = new MPI_Request[d];


    for (int i = 0; i < d; i++) {
        // Prepare meta data
        int duplicateIdx = childParent[rank][0];

        int source = childParent[rank][d + i];
        if (source != duplicateIdx) {
            // cout << "From rank: " << rank << " wait to recieve from: " << source << " numitems: " << d << endl;
            MPI_Irecv(recvbufv2[i], d * recvcount, MPI_FLOAT, source, 0, comm, &reqrecvs1[i]);
        }
        else {
            // if (rank ==0 ){
            //     cout << "From rank: " << rank << " wait to recieve from: " << source << " store position: " << i << endl;
            // }

            MPI_Irecv(recvbufv2[i], (d - 1) * recvcount, MPI_FLOAT, source, 100, comm, &reqrecvs1[i]);
        }
    }
    for (int i = 0; i < d; i++) {
        int duplicateIdx = childParent[rank][0];
        // Send data to children
        // For each child i 
        int destination = childParent[rank][i];
        if (destination != duplicateIdx) { // send d chunks of data

            // prepare data
            memcpy(&sendbufv2[i][0], &recvbuf1[0][(i - 1) * sendcount], sendcount * sizeof(float));
            for (int j = 1; j < d; j++) {
                memcpy(&sendbufv2[i][j * sendcount], &recvbuf1[j][i * sendcount], sendcount * sizeof(float));
            }
            MPI_Isend(sendbufv2[i], sendcount * d, MPI_FLOAT, destination, 0, comm, &reqsends1[i]);
            // send

        }
        else { // send d - 1 chunks of data
         // prepare data
            for (int j = 0; j < d - 1; j++) {
                memcpy(&sendbufv2[i][j * sendcount], &recvbuf1[j + 1][i], sendcount * sizeof(float));
            }

            MPI_Isend(sendbufv2[i], (d - 1) * sendcount, MPI_FLOAT, destination, 100, comm, &reqsends1[i]);

        }
    }
    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqsends1[i], MPI_STATUS_IGNORE);

    }

    for (int i = 0; i < d; i++) {
        // Wait for recieving complete
        MPI_Wait(&reqrecvs1[i], MPI_STATUS_IGNORE);
    }


    int parent, idx;
    for (int i = 0; i < d; i++) {
        // Copy to the final result
        if (i == 0) { // duplicate index
            // For each data chunk in the received buf

            for (int j = 0; j < d - 1; j++) {
                // calculate index of the data
                parent = childParent[rank][d + i];
                idx = childParent[parent][d + j + 1];
                // cout << "From rank: " << rank << " write data of " << parent << " to idx: " << idx << endl;
                // copy to the final result
                memcpy(&recvbuf[idx * recvcount], &recvbufv2[i][j * recvcount], recvcount * sizeof(float));
            }
        }
        else {
            for (int j = 0; j < d; j++) {
                // calculate index of the data
                parent = childParent[rank][d + i];
                idx = childParent[parent][d + j];
                // cout << "From rank: " << rank << " write data of " << parent << " to idx: " << idx << endl;
                // copy to the final result
                memcpy(&recvbuf[idx * recvcount], &recvbufv2[i][j * recvcount], recvcount * sizeof(float));
            }
        }
    }


    delete reqrecvs1;
    delete reqsends1;
    free(sendbufv2);
    for (int i = 1; i < d; i++) {
        free(recvbuf1[i]);
        free(recvbufv2[i]);
    }
    free(recvbufv2);
    free(recvbuf1);
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