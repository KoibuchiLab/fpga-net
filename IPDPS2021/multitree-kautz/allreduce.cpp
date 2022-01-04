/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-12-26 20:14:05
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-01-04 18:06:07
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


int main(int argc, char* argv[]) {
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

    // FILE* debug = stdout;
    // char filename1[256];
    // sprintf(filename1, "log/%d", rank);
    // debug = fopen(filename1, "w");
    
    char algo = COMBINE;
    double dblasttimer;
    // Calculate degree
    int d = (1 + sqrt(-1 + 4 * size)) / 2;
    char hostname[256];
    MPI_Get_processor_name(hostname, &hostname_len);
    char topo[256];
    // Topology optional argument
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--topo")) {
            if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%s", topo) != 1)) {
                program_abort("Invalid <topology> argument\n");
            }
            else { // prepare fake hostname for testing on MPICH (without simulation
                r2h_r(rank, d, hostname);
                hostname_len = strlen(hostname) + 1;
                int tmpd;
                sscanf(topo, "k%d", &tmpd);
                if (tmpd != d) {
                    program_abort("Invalid number of process\n");
                }

#if defined(DEBUG0)
                printf("Rank: %d | Host name: %s | Length: %d\n", rank, hostname, hostname_len);
#endif
            }
        }
        if (!strcmp(argv[i], "--num-item")) {
            if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%ld", &NUM_ITEMS) != 1)) {
                program_abort("Invalid num of items.\n");
            }
        }
        if (!strcmp(argv[i], "--algorithm")) {
            if (i + 1 <= argc) {
                if (!strcmp(argv[i + 1], "mttree")) {
                    algo = MULTITREE;
                }
                if (!strcmp(argv[i + 1], "conges")) {
                    algo = CONGESTION;
                }
                if (!strcmp(argv[i + 1], "cmb")) {
                    algo = COMBINE;
                }
            }
            else {
                program_abort("Enter algorithm.\n");
            }
        }
    }

#if defined(DEBUG0)
    printf("Hostname: %s | Rank: %d\n", hostname, rank);
#endif

    if (size != d * (d + 1)) {
        program_abort("Number of process must equal to # node Kautz graph diameter 2\n");
    }

    // Allocate memory for data
    size_t NUM_ITEMS_ORIGIN = NUM_ITEMS;
    NUM_ITEMS = ((NUM_ITEMS - 1) / size + 1) * size;
    // fprintf(debug, "NUM_ITEMS: %ld\n", NUM_ITEMS);
    size_t numofrsitems = NUM_ITEMS/size;

    float* data;
    if ((data = new float[NUM_ITEMS * size]) == NULL) {
        program_abort("Allocate fail!");
    }

    // All rank fill the buffer with random data
    srandom(RAND_SEED + rank);
    for (size_t j = 0; j < NUM_ITEMS; j++) {
        //data[i] = (float)((rank*1000 + i));
        if (j < NUM_ITEMS_ORIGIN){
            data[j] = (float)(1 + 1.0 * (random() % 9));//rank * 10 + j; //;
        } else {
            data[j] = 0;
        }
        
    }
    
    float* allreduceresult = (float*)malloc(sizeof(float) * NUM_ITEMS);

#if defined(DEBUG1)
    fprintf(stdout, "Data from rank %d: ", rank);
    for (int i = 0; i < NUM_ITEMS; i++) {
        fprintf(stdout, "%.0f\t", data[i]);
    }
    fprintf(stdout, "\n");
    MPI_Barrier(MPI_COMM_WORLD);
#endif

    // Start the timer
    double start_time, rstime;

    int groupnumber, nodenumber;
    r2h(rank, d, groupnumber, nodenumber);
    if (rank != h2r(hostname, d))
        program_abort("Computed rank must equal to real rank\n");

    int numofgroups = size / d;
    int numofnodesingroup = d;

    // size 2d + 1; last element is duplicate index
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
    filename = "scheduleTable/kout" + to_string(d);
    file.open(filename);
    // reduce memory used: each process hold one line of this table
    vector <Int3>scheduleTable;

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
    file.close();

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////	  ALLREDUCE : START	     ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    switch (algo) {
    case MULTITREE:{
        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                 REDUCE SCATTER                                          //

        
        // float* localbuf = (float*)malloc(sizeof(float) * d * numofrsitems);
        float *localbuf = (float*)malloc(sizeof(float)*d*numofrsitems);
        
        // float *rsbuf = (float*)malloc(sizeof(float)*d*numofrsitems);
        float *rsbuf = (float*)malloc(sizeof(float)*d*numofrsitems);
        // for (int i = 0; i < numofrsitems*d; i++){
        //     // should careful other reduce operations
        //     localbuf[i] = 0;
        //     rsbuf[i] = 0;
        // }
        memset(localbuf, 0.0f, sizeof(float)*d*numofrsitems);
        memset(rsbuf, 0.0f, sizeof(float)*d*numofrsitems);

        // Send and receive data for the first round
        MPI_Request *reqrecvfirst = (MPI_Request*)malloc(sizeof(MPI_Request)*d);
        MPI_Request *reqsendfirst = (MPI_Request*)malloc(sizeof(MPI_Request)*d);

        // Receive data from parent
        for (int i = 1; i < d; i++){
            int source = childParent[rank][d + i];
            // data index in rsbuf = i
            MPI_Irecv(&localbuf[i*numofrsitems], numofrsitems, MPI_FLOAT, source, \
                    0, MPI_COMM_WORLD, &reqrecvfirst[i]);
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // Step 0: send data on the first tree (d - 1 children): not send to duplicate idx childParent[rank][0]
        for (int i = 1; i < d; i++){
            int destination = childParent[rank][i];
            int dataindex = childParent[destination][0];
            MPI_Isend(&data[dataindex*numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                    MPI_COMM_WORLD, &reqsendfirst[i]);
        }

        for (int i = 1; i < d; i++){
            MPI_Wait(&reqrecvfirst[i], MPI_STATUS_IGNORE);
            for (int j = 0; j < numofrsitems; j++) {
                rsbuf[0*numofrsitems + j] += localbuf[i*numofrsitems + j];
            }
        }
        for (int i = 1; i < d; i++) {
            MPI_Wait(&reqsendfirst[i], MPI_STATUS_IGNORE);
        }
#if defined(DEBUG1)
    fprintf(debug, "Local result after the 0th step:\n\t");
    for (int i = 0; i < d*numofrsitems; i++){
        fprintf(debug, "%0.f ", localbuf[i]);
    }
    fprintf(debug, "\n");

    fprintf(debug, "rsbufs after the 0th step:\n\t");
    for (int i = 0; i < d*numofrsitems; i++) {
        fprintf(debug, "%0.f ", rsbuf[i]);
    }
    fprintf(debug, "\n");
    fprintf(debug, "\n");
#endif
        /////////////////////////////////////////////////////////////////////////////////////////
        // Step 1 -> d - 1: send to d - 1 trees, each tree has d children
        MPI_Request* reqrecvs;
        MPI_Request* reqsends;
        for (int step = 1; step < d; step++){
            reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * d);
            reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * d);

            // Receive data from d parents
            for (int i = 0; i < d; i++){
                int source = childParent[rank][d + i];
                // index in rsbuf equal to step
                MPI_Irecv(&localbuf[i*numofrsitems], numofrsitems, MPI_FLOAT, source, 0, \
                        MPI_COMM_WORLD, &reqrecvs[i]);
            }

            // Send data to d children
            for (int i = 0; i < d; i++){
                int destination = childParent[rank][i];
                int dataindex = childParent[destination][d - step];
                // fprintf(debug, "Step %d, destination %d, dataindex %d\n", step, destination, dataindex);
                MPI_Isend(&data[dataindex*numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                        MPI_COMM_WORLD, &reqsends[i]);
            }
            for (int i = 0; i < d; i++){
                MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
                for (int j = 0; j < numofrsitems; j++){
                    rsbuf[step*numofrsitems + j] += localbuf[i*numofrsitems + j];
                }
            }
            for (int i = 0; i < d; i++) {
                MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
            }
            free(reqsends);
            free(reqrecvs);

        
#if defined(DEBUG1)
    fprintf(debug, "Local result in the %dth step:\n\t", step);
    for (int i = 0; i < d * numofrsitems; i++) {
        fprintf(debug, "%0.f ", localbuf[i]);
    }
    fprintf(debug, "\n");

    fprintf(debug, "rsbufs after the %dth step:\n\t", step);
    for (int i = 0; i < d*numofrsitems; i++) {
        fprintf(debug, "%0.f ", rsbuf[i]);
    }
    fprintf(debug, "\n");
#endif
        }
        
        // Last step exchange data from rsbuf
        MPI_Request* reqrecvlast = (MPI_Request*)malloc(sizeof(MPI_Request) * d);
        MPI_Request* reqsendlast = (MPI_Request*)malloc(sizeof(MPI_Request) * d);

        // Receive rsbuf from parents
        for (int i = 0; i < d; i++){
            int source = childParent[rank][d + i];
            MPI_Irecv(&localbuf[i*numofrsitems], numofrsitems, MPI_FLOAT, source, 0, \
                    MPI_COMM_WORLD, &reqrecvlast[i]);
        }
        
        // Send rsbuf to d children
        for (int i = 0; i < d; i++){
            int destination = childParent[rank][d - i];
            for (int j = 0; j < numofrsitems; j++){
                rsbuf[i * numofrsitems + j] += data[destination*numofrsitems + j];
            }
            MPI_Isend(&rsbuf[i*numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                    MPI_COMM_WORLD, &reqsendlast[i]);
        }

        // allocate rsresult
        float *rsresult = (float*)malloc(sizeof(float)*numofrsitems);
        //Copy data from current process to the result;
        memcpy(rsresult, &data[rank*numofrsitems], sizeof(float)*numofrsitems);
#if defined(DEBUG1)
    fprintf(debug, "rsresult init: \n\t");
    for (int i = 0; i < numofrsitems; i++) {
        fprintf(debug, "%0.f ", rsresult[i]);
    }
    fprintf(debug, "\n");
#endif
        for (int i = 0; i < d; i++) {
            MPI_Wait(&reqrecvlast[i], MPI_STATUS_IGNORE);
            for (int j = 0; j < numofrsitems; j++){
                rsresult[j] += localbuf[i * numofrsitems + j];
            }
        }

        for (int i = 0; i < d; i++){
            MPI_Wait(&reqsendlast[i], MPI_STATUS_IGNORE);
        }
#if defined(DEBUG1)
    fprintf(debug, "Local result in the final step:\n\t");
    for (int i = 0; i < d * numofrsitems; i++) {
        fprintf(debug, "%0.f ", localbuf[i]);
    }
    fprintf(debug, "\n");
    
    fprintf(debug, "rsresult: \n\t");
    for (int i = 0; i < numofrsitems; i++) {
        fprintf(debug, "%0.f ", rsresult[i]);
    }
    fprintf(debug, "\n");
#endif
        free(reqrecvlast);
        free(reqsendlast);

        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                 END: REDUCE SCATTER                                     //

        rstime = MPI_Wtime();
        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                     ALLGATHER                                           //
        
        // float* allreduceresult = (float*)malloc(sizeof(float)*NUM_ITEMS);
        
        for (int i = 0; i < numofrsitems; i++) {
            allreduceresult[rank * numofrsitems + i] = rsresult[i];
        }
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
                    int sendIdx = scheduleTable[i].sendidx * numofrsitems;
                    int recvIdx = scheduleTable[i].src * numofrsitems;
                    //cout << "From rank " << rank << " destination: " << destination \
								<< " data index: " << sendIdx << endl;
                    MPI_Irecv(&allreduceresult[recvIdx], numofrsitems, MPI_FLOAT, source, \
                        0, MPI_COMM_WORLD, &reqrecvs[i]);
                    MPI_Isend(rsresult, numofrsitems, MPI_FLOAT, destination, \
                        0, MPI_COMM_WORLD, &reqsends[i]);
                }

                for (int i = 0; i < d; i++) {
                    MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
                }
                for (int i = 0; i < d; i++) {
                    MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
                }

#if defined(PRINT_STEP_0)
                // Print allgather result after step 0
                cout << "From rank " << rank << " allreduceresult after step 0:\n\t";
                for (int i = 0; i < numofrsitems * size; i++) {
                    cout << allreduceresult[i] << "\t";
                }
                cout << endl;
#endif
                delete reqrecvs;
                delete reqsends;
            } else if (step == 1) { //step = 1, then send to d - 1 node
                reqrecvs = new MPI_Request[d - 1];
                reqsends = new MPI_Request[d - 1];

                for (int i = 0; i < d - 1; i++) {
                    int source = scheduleTable[d + i].src;
                    int destination = scheduleTable[d + i].dst;
                    int sendIdx = scheduleTable[d + i].sendidx * numofrsitems;
                    int recvIdx = scheduleTable[d + i].recvidx * numofrsitems;
                    //printf("->> From rank %d, source: %d, dest: %d, sendidx: %d, recvidx: %d\n",\
								rank, source, destination, sendIdx, recvIdx);
                    MPI_Irecv(&allreduceresult[recvIdx], numofrsitems, MPI_FLOAT,
                        source, 0, MPI_COMM_WORLD, &reqsends[i]);
                    MPI_Isend(&allreduceresult[sendIdx], numofrsitems, MPI_FLOAT, \
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
#if defined(PRINT_STEP_1)
                // Print allgather result after step 1
                cout << "From rank " << rank << " allreduceresult after step 1 :\n\t";
                for (int i = 0; i < numofrsitems * size; i++) {
                    cout << allreduceresult[i] << "\t";
                }
                cout << endl;
#endif
            } else { // send to d nodes with different index
                reqrecvs = new MPI_Request[d];
                reqsends = new MPI_Request[d];

                for (int i = 0; i < d; i++) {
                    int source = scheduleTable[step * d + i - 1].src;
                    int destination = scheduleTable[step * d + i - 1].dst;
                    int sendIdx = scheduleTable[step * d + i - 1].sendidx * numofrsitems;
                    int recvIdx = scheduleTable[step * d + i - 1].recvidx * numofrsitems;

                    MPI_Irecv(&allreduceresult[recvIdx], numofrsitems, MPI_FLOAT, source, \
                        0, MPI_COMM_WORLD, &reqrecvs[i]);
                    MPI_Isend(&allreduceresult[sendIdx], numofrsitems, MPI_FLOAT, destination, \
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
        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                     END: ALLGATHER                                      //
            }
        }
        break; //optional
    } case CONGESTION:{
        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                      REDUCE SCATTER                                     //
        MPI_Request* reqrecvs0 = (MPI_Request*)malloc(sizeof(MPI_Request) * d * d);
        MPI_Request* reqsends0 = (MPI_Request*)malloc(sizeof(MPI_Request) * d * d);
        float **localbuf = (float**)malloc(sizeof(float*)*d);
        float* rsbuf = (float*)malloc(sizeof(float)*d*numofrsitems);
        for (int i = 0; i < d; i++){
            localbuf[i] = (float*)malloc(sizeof(float)*d*numofrsitems);
            memset(localbuf[i], 0.0f, sizeof(float)*d*numofrsitems);
        }
        memset(rsbuf, 0.0f, sizeof(float)* d* numofrsitems);

        // Receive data from parent
        for (int i = 1; i < d; i++) {
            int source = childParent[rank][d + i];
            // data index in rsbuf = i
            MPI_Irecv(&localbuf[0][i * numofrsitems], numofrsitems, MPI_FLOAT, source, \
                0, MPI_COMM_WORLD, &reqrecvs0[i]);
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // Step 0: send data on the first tree (d - 1 children): not send to duplicate idx childParent[rank][0]
        for (int i = 1; i < d; i++) {
            int destination = childParent[rank][i];
            int dataindex = childParent[destination][0];
            MPI_Isend(&data[dataindex * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                MPI_COMM_WORLD, &reqsends0[i]);
        }

        
        /////////////////////////////////////////////////////////////////////////////////////////
        // Step 1 -> d - 1: send to d - 1 trees, each tree has d children

        for (int step = 1; step < d; step++) {
            // Receive data from d parents
            for (int i = 0; i < d; i++) {
                int source = childParent[rank][d + i];
                // index in rsbuf equal to step
                MPI_Irecv(&localbuf[step][i * numofrsitems], numofrsitems, MPI_FLOAT, source, 0, \
                    MPI_COMM_WORLD, &reqrecvs0[step*d + i]);
            }

            // Send data to d children
            for (int i = 0; i < d; i++) {
                int destination = childParent[rank][i];
                int dataindex = childParent[destination][d - step];
                // fprintf(debug, "Step %d, destination %d, dataindex %d\n", step, destination, dataindex);
                MPI_Isend(&data[dataindex * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                    MPI_COMM_WORLD, &reqsends0[step*d + i]);
            }
            
#if defined(DEBUG1)
            fprintf(debug, "Local result in the %dth step:\n\t", step);
            for (int i = 0; i < d * numofrsitems; i++) {
                fprintf(debug, "%0.f ", localbuf[step][i]);
            }
            fprintf(debug, "\n");

            fprintf(debug, "rsbufs after the %dth step:\n\t", step);
            for (int i = 0; i < d * numofrsitems; i++) {
                fprintf(debug, "%0.f ", rsbuf[i]);
            }
            fprintf(debug, "\n");
#endif
        }
        for (int i = 1; i < d; i++) {
            MPI_Wait(&reqrecvs0[i], MPI_STATUS_IGNORE);
            for (int j = 0; j < numofrsitems; j++) {
                rsbuf[0 * numofrsitems + j] += localbuf[0][i * numofrsitems + j];
            }
        }
        for (int i = 1; i < d; i++) {
            MPI_Wait(&reqsends0[i], MPI_STATUS_IGNORE);
        }
#if defined(DEBUG1)
        fprintf(debug, "Local result after the 0th step:\n\t");
        for (int i = 0; i < d * numofrsitems; i++) {
            fprintf(debug, "%0.f ", localbuf[0][i]);
        }
        fprintf(debug, "\n");

        fprintf(debug, "rsbufs after the 0th step:\n\t");
        for (int i = 0; i < d * numofrsitems; i++) {
            fprintf(debug, "%0.f ", rsbuf[i]);
        }
        fprintf(debug, "\n");
        fprintf(debug, "\n");
#endif
        for (int step = 1; step < d; step++){
            for (int i = 0; i < d; i++) {
                MPI_Wait(&reqrecvs0[step * d + i], MPI_STATUS_IGNORE);
                for (int j = 0; j < numofrsitems; j++) {
                    rsbuf[step * numofrsitems + j] += localbuf[step][i * numofrsitems + j];
                }
            }
            for (int i = 0; i < d; i++) {
                MPI_Wait(&reqsends0[step * d + i], MPI_STATUS_IGNORE);
            }
        }

        // Last step exchange data from rsbuf
        MPI_Request* reqrecvlast = (MPI_Request*)malloc(sizeof(MPI_Request) * d);
        MPI_Request* reqsendlast = (MPI_Request*)malloc(sizeof(MPI_Request) * d);

        // Receive rsbuf from parents
        for (int i = 0; i < d; i++) {
            int source = childParent[rank][d + i];
            MPI_Irecv(&localbuf[0][i * numofrsitems], numofrsitems, MPI_FLOAT, source, 0, \
                MPI_COMM_WORLD, &reqrecvlast[i]);
        }

        // Send rsbuf to d children
        for (int i = 0; i < d; i++) {
            int destination = childParent[rank][d - i];
            for (int j = 0; j < numofrsitems; j++) {
                rsbuf[i * numofrsitems + j] += data[destination * numofrsitems + j];
            }
            MPI_Isend(&rsbuf[i * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                MPI_COMM_WORLD, &reqsendlast[i]);
        }

        // allocate rsresult
        float* rsresult = (float*)malloc(sizeof(float) * numofrsitems);
        //Copy data from current process to the result;
        memcpy(rsresult, &data[rank * numofrsitems], sizeof(float) * numofrsitems);
#if defined(DEBUG1)
        fprintf(debug, "rsresult init: \n\t");
        for (int i = 0; i < numofrsitems; i++) {
            fprintf(debug, "%0.f ", rsresult[i]);
        }
        fprintf(debug, "\n");
#endif
        for (int i = 0; i < d; i++) {
            MPI_Wait(&reqrecvlast[i], MPI_STATUS_IGNORE);
            for (int j = 0; j < numofrsitems; j++) {
                rsresult[j] += localbuf[0][i * numofrsitems + j];
            }
        }

        for (int i = 0; i < d; i++) {
            MPI_Wait(&reqsendlast[i], MPI_STATUS_IGNORE);
        }
#if defined(DEBUG1)
        fprintf(debug, "Local result in the final step:\n\t");
        for (int i = 0; i < d * numofrsitems; i++) {
            fprintf(debug, "%0.f ", localbuf[0][i]);
        }
        fprintf(debug, "\n");

        fprintf(debug, "rsresult: \n\t");
        for (int i = 0; i < numofrsitems; i++) {
            fprintf(debug, "%0.f ", rsresult[i]);
        }
        fprintf(debug, "\n");
#endif
        free(reqrecvlast);
        free(reqsendlast);

        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                   END:  REDUCE SCATTER                                  //

        rstime = MPI_Wtime();
        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                     ALLGATHER                                           //

        MPI_Request *reqrecvs = new MPI_Request[d];
        MPI_Request *reqsends = new MPI_Request[d];
        for (int i = 0; i < numofrsitems; i++) {
            allreduceresult[rank * numofrsitems + i] = rsresult[i];
        }

        for (int i = 0; i < d; i++) {
            int destination = scheduleTable[i].dst;
            int source = scheduleTable[i].src;
            int recvIdx = scheduleTable[i].src * numofrsitems;
            //cout << "From rank " << rank << " destination: " << destination \
						<< " data index: " << sendIdx << endl;
            MPI_Irecv(&allreduceresult[recvIdx], numofrsitems, MPI_FLOAT, source, \
                0, MPI_COMM_WORLD, &reqrecvs[i]);
            MPI_Isend(rsresult, numofrsitems, MPI_FLOAT, destination, \
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

#if defined(PRINT_STEP_0)
        // Print allgather result after step 0
        cout << "From rank " << rank << " allgatherresult after step 0:\n\t";
        for (int i = 0; i < numofrsitems * size; i++) {
            cout << allGatherResult[i] << "\t";
        }
        cout << endl;
#endif

        // step 1: the remaining nodes
        reqrecvs = new MPI_Request[size];
        reqsends = new MPI_Request[size];

        for (int i = d; i < size - 1; i++) {
            int source = scheduleTable[i].src;
            int recvIdx = scheduleTable[i].recvidx * numofrsitems;

            MPI_Irecv(&allreduceresult[recvIdx], numofrsitems, MPI_FLOAT, source, \
                0, MPI_COMM_WORLD, &reqrecvs[i]);
        }
        for (int i = d; i < size - 1; i++) {
            int destination = scheduleTable[i].dst;
            int sendIdx = scheduleTable[i].sendidx * numofrsitems;

            MPI_Isend(&allreduceresult[sendIdx], numofrsitems, MPI_FLOAT, destination, \
                0, MPI_COMM_WORLD, &reqsends[i]);
        }

        for (int i = d; i < size - 1; i++) {
            MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
        }
        for (int i = d; i < size - 1; i++) {
            MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
        }
        delete reqrecvs;
        delete reqsends;
        break;
    } case COMBINE:{
        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                      REDUCE SCATTER                                     //
        MPI_Request* reqrecvs0 = (MPI_Request*)malloc(sizeof(MPI_Request) * d * d);
        MPI_Request* reqsends0 = (MPI_Request*)malloc(sizeof(MPI_Request) * d * d);
        float** localbuf = (float**)malloc(sizeof(float*) * d);
        float* rsbuf = (float*)malloc(sizeof(float) * d * numofrsitems);
        for (int i = 0; i < d; i++) {
            localbuf[i] = (float*)malloc(sizeof(float) * d * numofrsitems);
            memset(localbuf[i], 0.0f, sizeof(float) * d * numofrsitems);
        }
        memset(rsbuf, 0.0f, sizeof(float) * d * numofrsitems);

        // Receive data from parent
        for (int i = 1; i < d; i++) {
            int source = childParent[rank][d + i];
            // data index in rsbuf = i
            MPI_Irecv(&localbuf[0][i * numofrsitems], numofrsitems, MPI_FLOAT, source, \
                0, MPI_COMM_WORLD, &reqrecvs0[i]);
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        // Step 0: send data on the first tree (d - 1 children): not send to duplicate idx childParent[rank][0]
        for (int i = 1; i < d; i++) {
            int destination = childParent[rank][i];
            int dataindex = childParent[destination][0];
            MPI_Isend(&data[dataindex * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                MPI_COMM_WORLD, &reqsends0[i]);
        }


        /////////////////////////////////////////////////////////////////////////////////////////
        // Step 1 -> d - 1: send to d - 1 trees, each tree has d children

        for (int step = 1; step < d; step++) {
            // Receive data from d parents
            for (int i = 0; i < d; i++) {
                int source = childParent[rank][d + i];
                // index in rsbuf equal to step
                MPI_Irecv(&localbuf[step][i * numofrsitems], numofrsitems, MPI_FLOAT, source, 0, \
                    MPI_COMM_WORLD, &reqrecvs0[step * d + i]);
            }

            // Send data to d children
            for (int i = 0; i < d; i++) {
                int destination = childParent[rank][i];
                int dataindex = childParent[destination][d - step];
                // fprintf(debug, "Step %d, destination %d, dataindex %d\n", step, destination, dataindex);
                MPI_Isend(&data[dataindex * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                    MPI_COMM_WORLD, &reqsends0[step * d + i]);
            }

#if defined(DEBUG1)
            fprintf(debug, "Local result in the %dth step:\n\t", step);
            for (int i = 0; i < d * numofrsitems; i++) {
                fprintf(debug, "%0.f ", localbuf[step][i]);
            }
            fprintf(debug, "\n");

            fprintf(debug, "rsbufs after the %dth step:\n\t", step);
            for (int i = 0; i < d * numofrsitems; i++) {
                fprintf(debug, "%0.f ", rsbuf[i]);
            }
            fprintf(debug, "\n");
#endif
        }
        for (int i = 1; i < d; i++) {
            MPI_Wait(&reqrecvs0[i], MPI_STATUS_IGNORE);
            for (int j = 0; j < numofrsitems; j++) {
                rsbuf[0 * numofrsitems + j] += localbuf[0][i * numofrsitems + j];
            }
        }
        for (int i = 1; i < d; i++) {
            MPI_Wait(&reqsends0[i], MPI_STATUS_IGNORE);
        }
#if defined(DEBUG1)
        fprintf(debug, "Local result after the 0th step:\n\t");
        for (int i = 0; i < d * numofrsitems; i++) {
            fprintf(debug, "%0.f ", localbuf[0][i]);
        }
        fprintf(debug, "\n");

        fprintf(debug, "rsbufs after the 0th step:\n\t");
        for (int i = 0; i < d * numofrsitems; i++) {
            fprintf(debug, "%0.f ", rsbuf[i]);
        }
        fprintf(debug, "\n");
        fprintf(debug, "\n");
#endif
        for (int step = 1; step < d; step++) {
            for (int i = 0; i < d; i++) {
                MPI_Wait(&reqrecvs0[step * d + i], MPI_STATUS_IGNORE);
                for (int j = 0; j < numofrsitems; j++) {
                    rsbuf[step * numofrsitems + j] += localbuf[step][i * numofrsitems + j];
                }
            }
            for (int i = 0; i < d; i++) {
                MPI_Wait(&reqsends0[step * d + i], MPI_STATUS_IGNORE);
            }
        }

        // Last step exchange data from rsbuf
        MPI_Request* reqrecvlast = (MPI_Request*)malloc(sizeof(MPI_Request) * d);
        MPI_Request* reqsendlast = (MPI_Request*)malloc(sizeof(MPI_Request) * d);

        // Receive rsbuf from parents
        for (int i = 0; i < d; i++) {
            int source = childParent[rank][d + i];
            MPI_Irecv(&localbuf[0][i * numofrsitems], numofrsitems, MPI_FLOAT, source, 0, \
                MPI_COMM_WORLD, &reqrecvlast[i]);
        }

        // Send rsbuf to d children
        for (int i = 0; i < d; i++) {
            int destination = childParent[rank][d - i];
            for (int j = 0; j < numofrsitems; j++) {
                rsbuf[i * numofrsitems + j] += data[destination * numofrsitems + j];
            }
            MPI_Isend(&rsbuf[i * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
                MPI_COMM_WORLD, &reqsendlast[i]);
        }

        // allocate rsresult
        float* rsresult = (float*)malloc(sizeof(float) * numofrsitems);
        //Copy data from current process to the result;
        memcpy(rsresult, &data[rank * numofrsitems], sizeof(float) * numofrsitems);
#if defined(DEBUG1)
        fprintf(debug, "rsresult init: \n\t");
        for (int i = 0; i < numofrsitems; i++) {
            fprintf(debug, "%0.f ", rsresult[i]);
        }
        fprintf(debug, "\n");
#endif
        for (int i = 0; i < d; i++) {
            MPI_Wait(&reqrecvlast[i], MPI_STATUS_IGNORE);
            for (int j = 0; j < numofrsitems; j++) {
                rsresult[j] += localbuf[0][i * numofrsitems + j];
            }
        }

        for (int i = 0; i < d; i++) {
            MPI_Wait(&reqsendlast[i], MPI_STATUS_IGNORE);
        }
#if defined(DEBUG1)
        fprintf(debug, "Local result in the final step:\n\t");
        for (int i = 0; i < d * numofrsitems; i++) {
            fprintf(debug, "%0.f ", localbuf[0][i]);
        }
        fprintf(debug, "\n");

        fprintf(debug, "rsresult: \n\t");
        for (int i = 0; i < numofrsitems; i++) {
            fprintf(debug, "%0.f ", rsresult[i]);
        }
        fprintf(debug, "\n");
#endif
        free(reqrecvlast);
        free(reqsendlast);

        /////////////////////////////////////////////////////////////////////////////////////////////
        //                                   END:  REDUCE SCATTER                                  //

        rstime = MPI_Wtime();
        
        MPI_Request *reqrecvs = new MPI_Request[d];
        MPI_Request *reqsends = new MPI_Request[d];

        for (int i = 0; i < numofrsitems; i++) {
            allreduceresult[rank * numofrsitems + i] = rsresult[i];
        }
        int source, destination, sendidx, recvidx;
        for (int i = 0; i < d; i++) {
            destination = childParent[rank][i];
            source = childParent[rank][d + i];
            sendidx = rank * numofrsitems;
            recvidx = childParent[rank][d + i] * numofrsitems;
            MPI_Irecv(&allreduceresult[recvidx], numofrsitems, MPI_FLOAT, source, 0, \
                MPI_COMM_WORLD, &reqrecvs[i]);
            MPI_Isend(&allreduceresult[sendidx], numofrsitems, MPI_FLOAT, destination, 0, \
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
        for (int i = 0; i < numofrsitems * size; i++) {
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
        float *sendbuf0 = new float[d*numofrsitems];
        float **recvbuf0  = new float*[d];
        for (int i = 0; i < d; i++){
            recvbuf0[i] = new float[d*numofrsitems];
        }
        //prepare sendbuf
        for (int j = 0; j < d; j++) {
            memcpy(&sendbuf0[j * numofrsitems], &allreduceresult[childParent[rank][d + j] * numofrsitems], sizeof(float) * numofrsitems);
            /*for (int k = 0; k < numofrsitems; k++){
                sendbuf[j*numofrsitems + k] = allGatherResult[childParent[rank][d + j]*numofrsitems + k];
            }*/
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
                MPI_Irecv(recvbuf0[i], numofrsitems * (d - 1), MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
            }
            else {
                MPI_Irecv(recvbuf0[i], numofrsitems * d, MPI_FLOAT, source, 0, MPI_COMM_WORLD, &reqrecvs[i]);
            }

            destination = childParent[rank][i];
            if (destination != duplicateIdx) {
                MPI_Isend(sendbuf0, numofrsitems * d, MPI_FLOAT, destination, 0, MPI_COMM_WORLD, &reqsends[i]);
            }
            else {
                tmpi = i;
            }
        }
        float* nsendbuf = new float[(d - 1) * numofrsitems];

        // send the message at the duplicate index
        bool detectduplicate = false;
        for (int j = 0; j < d; j++) {
            if (!detectduplicate) {
                if (childParent[rank][d + j] == duplicateIdx) {
                    detectduplicate = true;
                    continue;
                }
                memcpy(&nsendbuf[j * numofrsitems], &allreduceresult[childParent[rank][d + j] * numofrsitems], sizeof(float) * numofrsitems);
                // for (int k = 0; k < numofrsitems; k++)
                // 	nsendbuf[j*numofrsitems + k] = allGatherResult[childParent[rank][d + j]*numofrsitems + k];
            }
            else {
                // for (int k = 0; k < numofrsitems; k++)
                // 	nsendbuf[(j - 1)*numofrsitems + k] = allGatherResult[childParent[rank][d + j]*numofrsitems + k];
                memcpy(&nsendbuf[(j - 1) * numofrsitems], &allreduceresult[childParent[rank][d + j] * numofrsitems], sizeof(float) * numofrsitems);
            }
        }
        MPI_Isend(nsendbuf, numofrsitems * (d - 1), MPI_FLOAT, duplicateIdx, 0, MPI_COMM_WORLD, &reqsends[tmpi]);


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
                    // for (int k = 0; k < numofrsitems; k++){
                    // 	allGatherResult[whichData[j]*numofrsitems + k] = recvbuf[i][j*numofrsitems + k];
                    // }
                    memcpy(&allreduceresult[whichData[j] * numofrsitems], &recvbuf0[i][j * numofrsitems], sizeof(float) * numofrsitems);
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
                        // for (int k = 0; k < numofrsitems; k++){
                        // 	allGatherResult[whichData[j]*numofrsitems + k] = recvbuf[i][(j)*numofrsitems + k];
                        // }
                        memcpy(&allreduceresult[whichData[j] * numofrsitems], &recvbuf0[i][(j)*numofrsitems], sizeof(float) * numofrsitems);
                    }
                    else { // copy data with adjust index
                     // for (int k = 0; k < numofrsitems; k++){
                     // 	allGatherResult[whichData[j]*numofrsitems + k] = recvbuf[i][(j - 1)*numofrsitems + k];
                     // }
                        memcpy(&allreduceresult[whichData[j] * numofrsitems], &recvbuf0[i][(j - 1) * numofrsitems], sizeof(float) * numofrsitems);
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
        delete recvbuf0;
        delete nsendbuf;
        delete sendbuf0;
        delete whichData;
        
        break;
    }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////	  ALLREDUCE : END	     ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    /// Stop timer
    MPI_Barrier(MPI_COMM_WORLD);
    double kimrdtime;
    if(rank == 0){
        kimrdtime = MPI_Wtime() - start_time;
    }
    double kimrstime = rstime - start_time;
    if ((0 == rank)) {
        fprintf(stdout, "k%d,%.7lf,%ld\n", d, kimrdtime, NUM_ITEMS_ORIGIN);
    }
#if defined(COMPARE_BUILDIN)
    start_time = MPI_Wtime();
    float* resultlib = (float*)malloc(sizeof(float) * NUM_ITEMS);
    MPI_Allreduce(data, resultlib, NUM_ITEMS, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if ((0 == rank)) {
        fprintf(stdout, "k%d,%.7lf,%.7lf,%.7lf,%ld\n", d, kimrdtime, MPI_Wtime() - start_time, kimrstime, NUM_ITEMS);
    }

    // Compare the result
    if (rank == 0) {
        for (int i = 0; i < NUM_ITEMS; i++) {
            if ((allreduceresult[i]) != (resultlib[i])) {
                cout << "Index " << i << " kim: " << allreduceresult[i] << " lib " << resultlib[i] << endl;
                fprintf(stdout, "%s\n", "Alltoall wrong");
                break;
            }
        }
    }
#endif
    delete data;
    // fclose(debug);
    MPI_Finalize();

    return 0;
}