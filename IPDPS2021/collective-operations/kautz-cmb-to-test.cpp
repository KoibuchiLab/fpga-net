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
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double dblasttimer;
    // Calculate degree
    int d = (1 + sqrt(-1 + 4 * size)) / 2;

    char hostname[256];
    MPI_Get_processor_name(hostname, &hostname_len);
    
    printf("Hostname: %s | Rank: %d\n", hostname, rank);

    if (size != d * (d + 1)) {
        printf("Number of process must equal to # node Kautz graph diameter 2\n");
    }

    // Allocate memory for data
    float* data;
    if ((data = new float[sendcount]) == NULL) {
        printf("Out of memory!");
    }

#if defined(DEBUG1)
    printf("Data from rank %d: ", rank);
    for (int i = 0; i < NUM_ITEMS; i++) {
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
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    // Step o send data of this node to all child nodes
#if defined(TIME_FOR_EACH_STEP)
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        dblasttimer = MPI_Wtime();
    }
#endif
    reqrecvs = new MPI_Request[d];
    reqsends = new MPI_Request[d];
    int source, destination, sendidx, recvidx;
    for (int i = 0; i < d; i++) {
        destination = childParent[rank][i];
        source = childParent[rank][d + i];
        sendidx = rank * sendcount;
        recvidx = childParent[rank][d + i] * recvcount;
        MPI_Irecv(&allGatherResult[recvidx], recvcount, MPI_FLOAT, source, 0, \
            MPI_COMM_WORLD, &reqrecvs[i]);
        MPI_Isend(&allGatherResult[sendidx], sendcount, MPI_FLOAT, destination, 0, \
            MPI_COMM_WORLD, &reqsends[i]);
    }
    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
    }
    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
    }
    delete reqsends;
    delete reqrecvs;
#if defined(TIME_FOR_EACH_STEP)
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        fprintf(stdout, "%.7lf\tTime for step 0\n", MPI_Wtime() - dblasttimer);
    }
#endif

#if defined(PRINT_STEP_0)
    // Print allgather result after step 0
    cout << "From rank " << rank << " allgatherresult after step 0:\n\t";
    for (int i = 0; i < NUM_ITEMS * size; i++) {
        cout << allGatherResult[i] << "\t";
    }
    cout << endl;
#endif
    // Step 1 send receives informantion to all child node
    reqrecvs = new MPI_Request[d];
    reqsends = new MPI_Request[d];

    int duplicateIdx = childParent[rank][2 * d];
#if defined(TIME_FOR_EACH_STEP)
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        dblasttimer = MPI_Wtime();
    }
#endif
    //prepare sendbuf
    for (int j = 0; j < d; j++) {
        memcpy(&sendbuf0[j * sendcount], &allGatherResult[childParent[rank][d + j] * sendcount], sizeof(float) * sendcount);
    }
    int tmpi;
#if defined(TIME_FOR_EACH_STEP)
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        fprintf(stdout, "%.7lf\tCopy to send buf\n", MPI_Wtime() - dblasttimer);
        dblasttimer = MPI_Wtime();
    }
#endif

    // send and receive message
    for (int i = 0; i < d; i++) {
        source = childParent[rank][d + i];
        if (source == duplicateIdx) {
            MPI_Irecv(recvbuf0[i], recvcount * (d - 1), MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
        }
        else {
            MPI_Irecv(recvbuf0[i], recvcount * d, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
        }

        destination = childParent[rank][i];
        if (destination != duplicateIdx) {
            MPI_Isend(sendbuf0, sendcount * d, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);
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
    MPI_Isend(nsendbuf, sendcount * (d - 1), MPI_FLOAT, duplicateIdx, 0, MPI_COMM_WORLD, &reqsends[tmpi]);


    int* whichData = new int[d];
    // wait for send and reseive complete and copy buffer to final result
    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
    }


    for (int i = 0; i < d; i++) {
        MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
    }
#if defined(TIME_FOR_EACH_STEP)
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        fprintf(stdout, "%.7lf\tSend receive done\n", MPI_Wtime() - dblasttimer);
        dblasttimer = MPI_Wtime();
    }
#endif
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


#if defined(TIME_FOR_EACH_STEP)
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        fprintf(stdout, "%.7lf\tCopy to final result\n", MPI_Wtime() - dblasttimer);
        dblasttimer = MPI_Wtime();
    }
#endif
    for (int i = 0; i < d; i++) {
        delete recvbuf0[i];
    }
    delete nsendbuf;
    delete sendbuf0;
    delete whichData;
    return(MPI_SUCCESS);
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