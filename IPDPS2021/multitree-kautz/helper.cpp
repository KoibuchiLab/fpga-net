/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 12:02:49
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-12-21 14:14:18
 * @ Description:
 */

#include "helper.hpp"
#include "config.hpp"

int h2r(const char *hostname, int degree){
	int a, b;
	sscanf(hostname, "%d,%d", &a, &b);
	if(b > a){
		return degree*a + b - 1;
	} else {
		return degree*a + b;
	}	
}

int hidx2r(const int a, const int b, int degree){
	if(b > a){
		return degree*a + b - 1;
	} else {
		return degree*a + b;
	}
}

void r2h(int rank, int degree, int &groupnumber, int &nodenumber){
	groupnumber = rank/degree;
	nodenumber = rank%degree;
	if (nodenumber >= groupnumber) nodenumber++;
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
