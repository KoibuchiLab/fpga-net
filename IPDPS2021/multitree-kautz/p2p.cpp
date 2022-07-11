/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:33:06
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-07-11 17:13:10
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
#include <assert.h>

#include "helper.hpp"
#include "config.hpp"


using namespace std;
#define RAND_SEED 721311

vector <int>* childParent;
int algo = MULTIPATH;
int KMPI_Send(const void* buf, size_t count, MPI_Datatype datatype, int source, int dest, int tag, MPI_Comm comm);
int KMPI_Recv(void* buf, size_t count, MPI_Datatype datatype, int source, int dest, int tag, MPI_Comm comm);

int main(int argc, char* argv[]) {
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
            if ((i + 1 >= argc) || (sscanf(argv[i + 1], "%d", &NUM_ITEMS) != 1)) {
                program_abort("Invalid num of items.\n");
            }
        }
        if (!strcmp(argv[i], "--algorithm")) {
            if (i + 1 <= argc) {
                if (!strcmp(argv[i + 1], "naive")) {
                    algo = NAIVE;
                }
                if (!strcmp(argv[i + 1], "multipath")) {
                    algo = MULTIPATH;
                }
                if (!strcmp(argv[i + 1], "cmb")) {
                    algo = COMBINE;
                }

                if (!strcmp(argv[i + 1], "lib")) {
                    algo = LIB;
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
    float* data;
    if ((data = new float[NUM_ITEMS]) == NULL) {
        program_abort("Allocate fail!");
    }

    // All rank fill the buffer with random data
    int kim = d*(d - 1);
    NUM_ITEMS = (NUM_ITEMS / kim) * kim;
    srandom(RAND_SEED + rank);
    for (int j = 0; j < NUM_ITEMS; j++) {
        //data[i] = (float)((rank*1000 + i));
        data[j] = j; //(float)(1 + 1.0 * (random() % 9));
    }
#if defined(DEBUG1)
    printf("Data from rank %d: ", rank);
    for (int i = 0; i < NUM_ITEMS * size; i++) {
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

    // allocate memory for result NUM_ITEMS*size
    float* result = new float[NUM_ITEMS]();

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    double kimrdtime;
    /////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////	       P2P  : START	     ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    int a, b;
    int source = 0;
    int dest = 6;
    for (int s = 0; s < d + 1; s++){
        for (int ds = 0; ds < d + 1; ds++){
            if ( s != ds){
                if (rank == 0) {
                    // cout << "Source: " << s << ", Dest: " << ds << endl;
                }
                KMPI_Send(data, NUM_ITEMS, MPI_FLOAT, s, ds, 0, MPI_COMM_WORLD);
                KMPI_Recv(result, NUM_ITEMS, MPI_FLOAT, s, ds, 0, MPI_COMM_WORLD);
            }
        }
    }
    
    kimrdtime = MPI_Wtime() - start_time;
    if ((0 == rank)) {
        fprintf(stdout, "%d,0,k%d,%.7lf,%d\n", algo, d, kimrdtime, NUM_ITEMS);
    }
    /*
    int source = hidx2r(0, 1, d);
    int dest = hidx2r(1, 0, d);
    
    KMPI_Send(data, NUM_ITEMS, MPI_FLOAT, source, dest, 0, MPI_COMM_WORLD);
    KMPI_Recv(result, NUM_ITEMS, MPI_FLOAT, source, dest, 0, MPI_COMM_WORLD);
 
    /// Stop timer
    MPI_Barrier(MPI_COMM_WORLD);

    kimrdtime = MPI_Wtime() - start_time;
    if ((0 == rank)) {
        fprintf(stdout, "%d,0,k%d,%.7lf,%d\n",algo, d, kimrdtime, NUM_ITEMS);
    }
    /////////////////// egress links
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    dest = hidx2r(1, 2, d);
    KMPI_Send(data, NUM_ITEMS, MPI_FLOAT, source, dest, 0, MPI_COMM_WORLD);
    KMPI_Recv(result, NUM_ITEMS, MPI_FLOAT, source, dest, 0, MPI_COMM_WORLD);
    /// Stop timer
    MPI_Barrier(MPI_COMM_WORLD);
    kimrdtime = MPI_Wtime() - start_time;
    if ((0 == rank)) {
        fprintf(stdout, "%d,1,k%d,%.7lf,%d\n", algo, d, kimrdtime, NUM_ITEMS);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        start_time = MPI_Wtime();
    }
    
    // ///////////////////////////// ingress links
    dest = hidx2r(2, 0, d);
    KMPI_Send(data, NUM_ITEMS, MPI_FLOAT, source, dest, 0, MPI_COMM_WORLD);
    KMPI_Recv(result, NUM_ITEMS, MPI_FLOAT, source, dest, 0, MPI_COMM_WORLD);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////  	    P2P : END	     ////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    /// Stop timer
    MPI_Barrier(MPI_COMM_WORLD);
    kimrdtime = MPI_Wtime() - start_time;
    if ((0 == rank)) {
        fprintf(stdout, "%d,2,k%d,%.7lf,%d\n", algo, d, kimrdtime, NUM_ITEMS);
    }
    */
    delete data;
    delete result;
    MPI_Finalize();

    return 0;
}

int KMPI_Send(const void* buf, size_t count, MPI_Datatype datatype, int source, int dest, int tag, MPI_Comm comm){
    int size, rank;
    
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
    int d = (1 + sqrt(-1 + 4 * size)) / 2;
    int groupnumber, nodenumber, s_groupnumber, s_nodenumber, d_groupnumber, d_nodenumber;
    r2h(rank, d, groupnumber, nodenumber);
    r2h(source, d ,s_groupnumber, s_nodenumber);
    r2h(dest, d, d_groupnumber, d_nodenumber);
    
    // cout << "Hello\n";
    switch (algo) {
    case NAIVE: {
        // 2 kinds of send: have direct link and non
        if (havedirlink(source, dest, d)){
            if (rank == source) {
                MPI_Send(buf, count, datatype, dest, tag, comm);
            }
        } else { // send to intermediate node
            int internode = getinternode(source, dest, d);
            if (rank == source) MPI_Send(buf, count, MPI_FLOAT, internode, tag, comm);
            if (rank == internode) {
                float *tmp = new float[count];
                MPI_Recv(tmp, count, MPI_FLOAT, source, tag, comm, MPI_STATUS_IGNORE);
                MPI_Send(tmp, count, datatype, dest, tag, comm);
                delete tmp;
            }
        }
        break;
    }case MULTIPATH: {
        // step 0: Identify the kind of division: if command
        // 2: two directional link
        // 1: one directional link
        // 0: no direct link

        // # 3 kinds of connection: 2-directional link, outgoing link, no ingoing link
        if (havedirlink(source, dest, d) == 2) { // 2 directional link`
            // Step 1: send to all neighbor nodes
            // Hostname: groupnumber.nodenumber
            // Send to all (d) neighbors: nodenumber.i 
            // If i == groupnumber -> destination -> save data (on destination side)
            // else: send to intermediate nodes and forward to destination
            if (rank == source){ // send from source to all neighbors
                MPI_Request* reqsends;
                reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * (d + 1));
                float* tmpptr = (float*)buf; // WARNING
                for (int i = 0; i < d + 1; i++) {
                    // Split the data
                    // WARNING: devide the data wrong example KAUTZ(3,2) from process 7 to process 4 amount = 16
                    size_t amount = round((1.0 * count / d));
                    if (i == d) amount = count - (d - 1) * amount;
                    size_t position;
                    if (i < nodenumber){
                        position = round(i * (1.0 * count / d));
                    } else {
                        position = round((i - 1) * (1.0 * count / d));
                    }
                    
                    // Send to neighbor
                    //      Calculate target rank
                    //      Target rank = this.nodenumber and i
                    if (i != nodenumber){
                        int destination = hidx2r(nodenumber, i, d);
                        
                        // cout << "Process " << rank << " send the data to " << destination << " begin at " << position << " amount " << amount << endl;
                        MPI_Isend(&tmpptr[position], amount, datatype, destination, tag, comm, &reqsends[i]);
                    }
                }

                for (int i = 0; i < d + 1; i++){
                    if (i != nodenumber) MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
                }
                free(reqsends);
            }

            // Step 2: receive the data from source and forward to intermediate node
            if (groupnumber == s_nodenumber){ // Neighbor of source node -> recieve data
                MPI_Request reqrecv;
                size_t amount = round((1.0 * count / d));
                if (nodenumber == d) amount = count - (d - 1) * amount;
                if (nodenumber != s_groupnumber){ // receive to result: see KMPI_Recv function
                    float* bufforthedatafromsource = (float*)malloc(sizeof(float) * amount); // WARNING
                    // receive from the source then forwart to one intermediate node
                    
                    MPI_Recv(bufforthedatafromsource, amount, datatype, source, tag, comm, MPI_STATUS_IGNORE);
                    // cout << "Process " << rank << " recv the data from " << source << " amount " << amount << endl;

                    // forward to the intermediate node
                    int internode = hidx2r(nodenumber, d_groupnumber, d);
                    // cout << "Process " << rank << " send the data to " << internode << " amount " << amount << endl;
                    MPI_Send(bufforthedatafromsource, amount, datatype, internode, tag, comm);
                    free(bufforthedatafromsource);
                }
            }
            // Step 3: receive the data from source neighbors and forward to destination
            if (nodenumber == d_groupnumber && groupnumber != s_groupnumber){ // intermediate nodes
                // receive data
                // Calculate parent -> amount of data
                int parent_groupnumber, parent_nodenumber, parent_rank;
                parent_groupnumber = s_nodenumber;
                parent_nodenumber  = groupnumber;
                parent_rank = hidx2r(parent_groupnumber, parent_nodenumber, d);

                size_t amount = round(1.0 * count/d);
                if (groupnumber == d) amount = count - (d - 1)*amount;
                float *bufferinternode = (float*)malloc(sizeof(float)*amount);
                
                MPI_Recv(bufferinternode, amount, datatype, parent_rank, tag, comm, MPI_STATUS_IGNORE);
                // cout << "Process " << rank << " recv the data from " << parent_rank << " amount " << amount << endl;

            
                // forward to destination
                // cout << "Process " << rank << " send the data to " << dest << " amount " << amount << endl;
                MPI_Send(bufferinternode, amount, datatype, dest, tag, comm);
                free(bufferinternode);
            }
        } else if (havedirlink(source, dest, d) == 1){ // have unidirectional link
            // Step 1: divide the data into d - 1 parts. Send to d - 1 neighbor (except 2-dir-link node)
            if (rank == source){
                MPI_Request* reqsends;
                reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * (d + 1));
                int adjustindex = 0;
                float* tmpptr = (float*)buf; // WARNING

                for (int i = 0; i < d + 1; i++){
                    int amount, position;
                    if (i == s_nodenumber) adjustindex++; // no exist link
                    if (i == s_groupnumber) adjustindex++; // dont sent to bi-dir link
                    amount = round(1.0 * (count - 1) / (d - 1));
                    position = amount*(i - adjustindex);
                    // cout << "0Process " << rank << " i: " << i << " amount " << amount << " adjust index " << adjustindex << endl;
                    if (i == d) amount = count - (d - 2)*amount;
                    

                    int destination = hidx2r(s_nodenumber, i, d);
                    if (i != s_nodenumber && i != s_groupnumber){ // except node ii and node have bidirectional link
                        // cout << "0Process " << rank << " i: " << i << " amount " << amount << " adjust index " << adjustindex << endl;
                        // cout << "0Process " << rank << " send the data to " << destination << " begin at " << position << " amount " << amount << endl;
                        MPI_Isend(&tmpptr[position], amount, datatype, destination, tag, comm, &reqsends[i]);
                    }
                }
                for (int i = 0; i < d + 1; i++){
                    if (i != s_nodenumber && i != s_groupnumber){
                        MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
                    }
                }
                free(reqsends);
            }
            // Step 2: From neighbor nodes, send to intermediate nodes
            if (groupnumber == s_nodenumber && nodenumber != d_nodenumber && nodenumber != s_groupnumber){ // Neighbor
                int amount, adjustindex;
                if (nodenumber > s_nodenumber) adjustindex++;
                if (nodenumber == s_groupnumber) adjustindex++;
                amount = round(1.0 * (count - 1) / (d - 1));
                if (nodenumber == d) amount = count - (d - 2) * amount;
                float *tmpbuf = (float*)malloc(sizeof(float)*amount); // WARNING
                
                // cout << "11Process " << rank << " wait to receive the data from " << source << " amount " << amount << endl;
                MPI_Recv(tmpbuf, amount, datatype, source, tag, comm, MPI_STATUS_IGNORE);
                // cout << "11Process " << rank << " recv the data from " << source << " amount" << amount << endl;

                //Forward to intermediate node
                int internode = hidx2r(nodenumber, d_groupnumber, d);
                // cout << "11Process " << rank << " send the data to " << internode << " amount " << amount << endl;
                MPI_Send(tmpbuf, amount, datatype, internode, tag, comm);
                free(tmpbuf);
            }

            // Step 3: From intermediate node send to destination
            if (nodenumber == d_groupnumber && groupnumber != d_groupnumber && groupnumber != s_groupnumber){
                // Receive data from source neighbor
                int amount, adjustindex;
                if (groupnumber > s_nodenumber) adjustindex++;
                if (nodenumber == s_groupnumber) adjustindex++;
                amount = round(1.0 * (count - 1) / (d - 1));
                if (groupnumber == d) amount = count - (d - 2) * amount;
                float* tmpbuf = (float*)malloc(sizeof(float) * amount); // WARNING

                int parent = hidx2r(s_nodenumber, groupnumber, d);
                if (groupnumber != d_nodenumber){
                    // cout << "222Process " << rank << " wait to receive the data from " << parent << " amount " << amount << endl;
                    MPI_Recv(tmpbuf, amount, datatype, parent, tag, comm, MPI_STATUS_IGNORE);
                    // cout << "222Process " << rank << " recv the data from " << parent << " amount " << amount << endl;

                    // Forward to destination
                    // cout << "222Process " << rank << " send the data to " << dest << " amount " << amount << endl;
                    MPI_Send(tmpbuf, amount, datatype, dest, tag, comm);
                }
                free(tmpbuf);
            }
        } else if (havedirlink(source, dest, d) == 0) {
            // Step 1: from source send to neighbor nodes
            if (rank == source){
                MPI_Request* reqsends;
                reqsends = (MPI_Request*)malloc(sizeof(MPI_Request) * (d + 1));
                int adjustindex = 0;
                float* tmpptr = (float*)buf; // WARNING
                // Send to neighnor nodes of source
                // S_nodenumber:i
                for (int i = 0; i < d + 1; i++){
                    int amount, position;
                    if (i == s_nodenumber) adjustindex++;
                    amount = count / d;
                    position = amount*(i - adjustindex);
                    if (i == d) amount = count - amount*(d - 1);
                    if (i != s_nodenumber){
                        int destination = hidx2r(s_nodenumber, i, d);
                        // cout << "0Process " << rank << " send the data to " << destination << " begin at " << position << " amount " << amount << endl;
                        MPI_Isend(&tmpptr[position], amount, datatype, destination, tag, comm, &reqsends[i]);
                    } 
                }
                for (int i = 0; i < d + 1; i++){
                    if (i !=  nodenumber){
                        MPI_Wait(&reqsends[i], MPI_STATUS_IGNORE);
                    }
                }  
                free(reqsends);
            }
            // Step 2: receive from source node and forward to intermediate nodes
            if (groupnumber == s_nodenumber){
                int adjustindex = 0, amount;
                if (nodenumber > s_nodenumber) adjustindex++;
                assert(adjustindex == 0 || adjustindex == 1);
                amount = count / d;
                if (nodenumber == d) amount = count - (d - 1)*amount;
                float *tmpbuf = (float*)malloc(sizeof(float)*amount); // warning

                // Receive from source node
                // cout << "11Process " << rank << " wait to receive the data from " << source << " amount " << amount << endl;
                MPI_Recv(tmpbuf, amount, datatype, source, tag, comm, MPI_STATUS_IGNORE);
                // cout << "11Process " << rank << " recv data from " << source << " amount " << amount << endl;

                if (nodenumber == d_groupnumber){
                    // send to destination
                    // cout << "11Process " << rank << " send data to " << dest << " amount " << amount << endl;
                    MPI_Send(tmpbuf, amount, datatype, dest, tag, comm);
                } else {
                    // Forward to intermediate nodes
                    int internode = hidx2r(nodenumber, d_groupnumber, d);
                    // cout << "11Process " << rank << " send data to " << internode << " amount " << amount << endl;
                    MPI_Send(tmpbuf, amount, datatype, internode, tag, comm);
                }
                free(tmpbuf);
            }

            // Step 3: from inter node receive data and forward to destination
            if (nodenumber == d_groupnumber && groupnumber != s_nodenumber){
                int adjustindex = 0, amount;
                if (groupnumber > d_groupnumber) adjustindex++;
                assert(adjustindex == 0 || adjustindex == 1);
                amount = count / d;
                if (groupnumber == d) amount = count - (d - 1)*amount;// warning: dest have group number = d
                float* tmpbuf = (float*)malloc(sizeof(float) * amount); // warning
                int neighbor = hidx2r(s_nodenumber, groupnumber, d);
                // cout << "222Process " << rank << " wait to receive the data from " << neighbor << " amount " << amount << endl;
                MPI_Recv(tmpbuf, amount, datatype, neighbor, tag, comm, MPI_STATUS_IGNORE);
                // cout << "222Process " << rank << " recv data from " << neighbor << " amount " << amount << endl;

                // Forward to destination
                // cout << "222Process " << rank << " send data to " << dest << " amount " << amount << endl;
                MPI_Send(tmpbuf, amount, datatype, dest, tag, comm);
                free(tmpbuf);
            }
        } else {
            printf ("Review algorithm!!!!!!!!!!!!!!!!!!!!!!\n");
        }

        break;
    } case LIB:{
        if (rank == source){
            MPI_Send(buf, count, datatype, dest, tag, comm);
        }
    } 
    default: {
        cout << "Default algorithm\n";
        break;
    }
    }
    return MPI_SUCCESS;
}

int KMPI_Recv(void* buf, size_t count, MPI_Datatype datatype, int source, int dest, int tag, MPI_Comm comm){
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int d = (1 + sqrt(-1 + 4 * size)) / 2;
    int groupnumber, nodenumber, s_groupnumber, s_nodenumber, d_groupnumber, d_nodenumber;
    r2h(rank, d, groupnumber, nodenumber);
    r2h(source, d, s_groupnumber, s_nodenumber);
    r2h(dest, d, d_groupnumber, d_nodenumber);

    switch (algo) {
    case NAIVE: {
        // 2 kinds of send: have direct link and non
        if (havedirlink(source, dest, d)) {
            if (rank == dest) MPI_Recv(buf, count, datatype, source, tag, comm, MPI_STATUS_IGNORE);
        } else { // send to intermediate node
            int internode = getinternode(source, dest, d);
            if (rank == dest){
                MPI_Recv(buf, count, datatype, internode, tag, comm, MPI_STATUS_IGNORE);
            }
        }

        break;
    }case MULTIPATH: {
        // step 0: Identify the kind of division: if command
        // 2: two directional link
        // 1: one directional link
        // 0: no direct link

        // # 3 kinds of connection: 2-directional link, outgoing link, no ingoing link
        if (havedirlink(source, dest, d) == 2){ // 2 directional link`
            // Recv from all neighbor
            if (rank == dest){
                MPI_Request *reqrecvs;
                reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*(d + 1));
                size_t amount = round((1.0 * count / d));
                float* tmpptr = (float*)buf; // WARNING
                for (int i = 0; i < d + 1; i++){
                    if (i == d) amount = count - (d - 1) * amount;
                    size_t position;
                    if (i < d_groupnumber ){
                        position = round(i * (1.0 * count / d));
                    } else {
                        position = round((i - 1) * (1.0 * count / d));
                    }
                    
                    if (i != d_groupnumber){
                        int parent_node = hidx2r(i, d_groupnumber, d);
                        // cout << " Process " << rank << " waits to recv the data from " << parent_node << " amount " << amount << endl;
                        
                        // MPI_Recv(&tmpptr[position], amount, datatype, parent_node, tag, comm, MPI_STATUS_IGNORE);
                        MPI_Irecv(&tmpptr[position], amount, datatype, parent_node, tag, comm, &reqrecvs[i]);
                        // cout << "Process " << rank << " recv the data from " << parent_node << " begin at " << position << " amount " << amount << endl;
                    } 
                }
                for (int i = 0; i < d + 1; i++){
                    if(i != d_groupnumber){
                        MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);                      
                    }
                }
                free(reqrecvs);
            }

        } else if (havedirlink(source, dest, d) == 1){
            MPI_Request *reqrecvs;
            reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request)*(d + 1));
            float* tmpptr = (float*)buf; // WARNING
            if (rank == dest){
                int adjustindex = 0;
                for (int i = 0; i < d + 1; i++) {
                    int amount, position;
                    if (i == d_nodenumber) adjustindex++;
                    if (i == d_groupnumber) adjustindex++;
                    amount = round(1.0 * (count - 1) / (d - 1));
                    position = amount * (i - adjustindex);
                    if (i == d) amount = count - (d - 2) * amount;

                    if (i != d_groupnumber && i != d_nodenumber){
                        int parentnode = hidx2r(i, d_groupnumber, d);
                        // cout << "RProcess " << rank << " waits to recv the data from " << parentnode << " amount " << amount << endl;
                        // MPI_Recv(&tmpptr[position], amount, datatype, parentnode, tag, comm, MPI_STATUS_IGNORE);
                        MPI_Irecv(&tmpptr[position], amount, datatype, parentnode, tag, comm, &reqrecvs[i]);
                        // cout << "RProcess " << rank << " recv the data from " << parentnode << " begin at " << position << " amount " << amount << endl;
                    }
                    
                }
                for (int i = 0; i < d + 1; i++){
                    if( i != d_nodenumber && i != d_groupnumber){
                        MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);
                    }
                }
            }
            free(reqrecvs);
        } else if (havedirlink(source, dest, d) == 0) {
            MPI_Request* reqrecvs;
            reqrecvs = (MPI_Request*)malloc(sizeof(MPI_Request) * (d + 1));
            float* tmpptr = (float*)buf; // WARNING
            if (rank == dest){
                for (int i = 0; i < d + 1; i++) {
                    if (i != d_groupnumber) {
                        int amount, position, adjustindex = 0;
                        if (i > d_groupnumber) adjustindex++;
                        assert(adjustindex == 0 || adjustindex == 1);
                        int internode = hidx2r(i, d_groupnumber, d);
                        amount = count / d;
                        position = amount * (i - adjustindex);
                        if (i == d) amount = count - (d - 1) * amount;
                        // cout << "RProcess " << rank << " waits to recv the data from " << internode << " amount " << amount << endl;
                        // MPI_Recv(&tmpptr[position], amount, datatype, internode, tag, comm, MPI_STATUS_IGNORE);
                        MPI_Irecv(&tmpptr[position], amount, datatype, internode, tag, comm, &reqrecvs[i]);
                        // cout << "RProcess " << rank << " recv the data from " << internode << " begin at " << position << " amount " << amount << endl;

                    }
                }
                for (int i = 0; i < d + 1; i++){
                    if (i != d_groupnumber){
                        MPI_Wait(&reqrecvs[i], MPI_STATUS_IGNORE);   
                    }
                }
            }
            free(reqrecvs);
            
        } else {
            printf("Review algorithm!!!!!!!!!!!!!!!!!!!!!!");
        }

        break;
    } case LIB:{
        if (rank == dest) {
            MPI_Recv(buf, count, datatype, dest, tag, comm, MPI_STATUS_IGNORE);
        }
    } 
    default: {
        cout << "Default algorithm\n";
        break;
    }
    }
    return MPI_SUCCESS;
}