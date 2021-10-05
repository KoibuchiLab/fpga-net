/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 12:02:49
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-05 14:47:38
 * @ Description:
 */

#include "helper.hpp"
#include "config.hpp"

int h2r(char *hostname, int degree){
    return 0;
}

void r2h(int rank, int degree, int &a, int &b){

    return;
}

void r2h_r(int rank, int degree, char *hostname){
    int groupnumber = rank/degree;
    int nodenumber = rank%degree;

    if (nodenumber >= groupnumber) nodenumber++;
    sprintf(hostname, "%d,%d", groupnumber, nodenumber);
    return;
}

void program_abort(const char * message) {
	fprintf(stderr, "Error: %s", message);
    exit(1);
}