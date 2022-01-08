#include "kmpi.hpp"

extern vector <int>* childParent;
extern vector <Int3> scheduleTable;

int KMPI_Init(int rank, int size) {
    int d = (1 + sqrt(-1 + 4 * size)) / 2;
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
    return MPI_SUCCESS;
}


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
    printf("Hostname: %s | Rank: %d | Size: %d\n", hostname, rank, size);
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

    // copy local data to result
    for (int i = 0; i < sendcount; i++) {
        recvbuf[rank * sendcount + i] = sendbuf[i];
    }

    

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
        MPI_Irecv(&recvbuf[recvidx], recvcount, MPI_FLOAT, source, 0, \
            MPI_COMM_WORLD, &reqrecvs[i]);
        MPI_Isend(&recvbuf[sendidx], sendcount, MPI_FLOAT, destination, 0, \
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
    cout << "From rank " << rank << " recvbuf after step 0:\n\t";
    for (int i = 0; i < NUM_ITEMS * size; i++) {
        cout << recvbuf[i] << "\t";
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
        for (int k = 0; k < sendcount; k++) {
            sendbuf0[j * sendcount + k] = recvbuf[childParent[rank][d + j] * sendcount + k];
        }
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
            for (int k = 0; k < sendcount; k++)
                nsendbuf[j * sendcount + k] = recvbuf[childParent[rank][d + j] * sendcount + k];
        }
        else {
            for (int k = 0; k < sendcount; k++)
                nsendbuf[(j - 1) * sendcount + k] = recvbuf[childParent[rank][d + j] * sendcount + k];
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
                for (int k = 0; k < recvcount; k++) {
                    recvbuf[whichData[j] * recvcount + k] = recvbuf0[i][j * recvcount + k];
                }
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
                    for (int k = 0; k < recvcount; k++) {
                        recvbuf[whichData[j] * recvcount + k] = recvbuf0[i][(j)*recvcount + k];

                    }
                }
                else { // copy data with adjust index
                    for (int k = 0; k < recvcount; k++) {
                        recvbuf[whichData[j] * recvcount + k] = recvbuf0[i][(j - 1) * recvcount + k];

                    }
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
    delete(recvbuf0);
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
    return(MPI_SUCCESS);
}

int KMPI_Allreducef(const float* sendbuf, float* recvbuf, int count,
    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm){
    int rank;
    int size;
    int hostname_len;
    size_t NUM_ITEMS = 1011;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double dblasttimer;
    // Calculate degree
    int d = (1 + sqrt(-1 + 4 * size)) / 2;
    char hostname[256];
    MPI_Get_processor_name(hostname, &hostname_len);
    char topo[256];
    // Topology optional argument

#if defined(DEBUG0)
    printf("Hostname: %s | Rank: %d\n", hostname, rank);
#endif

    if (size != d * (d + 1)) {
        program_abort("Number of process must equal to # node Kautz graph diameter 2\n");
    }

    // fprintf(debug, "NUM_ITEMS: %ld\n", NUM_ITEMS);
    size_t numofrsitems = NUM_ITEMS / size;
    NUM_ITEMS = numofrsitems*size;

    float* allreduceresult = recvbuf;

    // Start the timer
    double start_time, rstime;

    int groupnumber, nodenumber;
    r2h(rank, d, groupnumber, nodenumber);
    if (rank != h2r(hostname, d))
        program_abort("Computed rank must equal to real rank\n");

    int numofgroups = size / d;
    int numofnodesingroup = d;

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
    
    /////////////////////////////////////////////////////////////////////////////////////////////
        //                                      REDUCE SCATTER                                     //
    MPI_Request* reqrecvs0 = (MPI_Request*)malloc(sizeof(MPI_Request) * d * d);
    MPI_Request* reqsends0 = (MPI_Request*)malloc(sizeof(MPI_Request) * d * d);
    float** localbuf = (float**)malloc(sizeof(float*) * d);
    float* rsbuf = (float*)SMPI_SHARED_MALLOC(sizeof(float) * d * numofrsitems);
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
        MPI_Isend(&sendbuf[dataindex * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
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
            MPI_Isend(&sendbuf[dataindex * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
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
            rsbuf[i * numofrsitems + j] += sendbuf[destination * numofrsitems + j];
        }
        MPI_Isend(&rsbuf[i * numofrsitems], numofrsitems, MPI_FLOAT, destination, 0, \
            MPI_COMM_WORLD, &reqsendlast[i]);
    }

    // allocate rsresult
    float* rsresult = (float*)SMPI_SHARED_MALLOC(sizeof(float) * numofrsitems);
    //Copy data from current process to the result;
    memcpy(rsresult, &sendbuf[rank * numofrsitems], sizeof(float) * numofrsitems);
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

    MPI_Request* reqrecvs = new MPI_Request[d];
    MPI_Request* reqsends = new MPI_Request[d];

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
    float* sendbuf0 = (float*)SMPI_SHARED_MALLOC(sizeof(float) * d * numofrsitems);
    float** recvbuf0 = new float* [d];
    for (int i = 0; i < d; i++) {
        recvbuf0[i] = new float[d * numofrsitems];
    }
    //prepare sendbuf
    for (int j = 0; j < d; j++) {
        memcpy(&sendbuf0[j * numofrsitems], &allreduceresult[childParent[rank][d + j] * numofrsitems], sizeof(float) * numofrsitems);
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
    float* nsendbuf = sendbuf0;

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
                memcpy(&allreduceresult[whichData[j] * numofrsitems], &recvbuf0[i][j * numofrsitems], sizeof(float) * numofrsitems);
            }
        } else {
            detectduplicate = false;
            for (int j = 0; j < d; j++) {
                if (whichData[j] == childParent[duplicateIdx][2 * d]) {
                    detectduplicate = true;
                    continue;
                }
                if (!detectduplicate) {						
                    memcpy(&allreduceresult[whichData[j] * numofrsitems], &recvbuf0[i][(j)*numofrsitems], sizeof(float) * numofrsitems);
                } else { 
                    memcpy(&allreduceresult[whichData[j] * numofrsitems], &recvbuf0[i][(j - 1) * numofrsitems], sizeof(float) * numofrsitems);
                }
            }

        }
    }
    return MPI_SUCCESS;
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

