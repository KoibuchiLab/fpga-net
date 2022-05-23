/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 12:02:49
 * @ Modified by: Kien Pham
 * @ Modified time: 2022-05-05 17:22:55
 * @ Description:
 */

#include "helper.hpp"
#include "config.hpp"

int getinternode(const int src, const int dst, const int degree) {
	int srca, srcb, dsta, dstb;
	r2h(src, degree, srca, srcb);
	r2h(dst, degree, dsta, dstb);
	return hidx2r(srcb, dsta, degree);
}
/*
return 0: no direct link 
return 1: one directional link
return 2: two directional link
*/
int havedirlink(const int src, const int dst, const int degree){
	int srca, srcb, dsta, dstb;
	r2h(src, degree, srca, srcb);
	r2h(dst, degree, dsta, dstb);
	if (srcb == dsta) {
		if (srca == dstb){
			return 2;
		}
		return(1);
	}	
	
	else return(0);
}

int h2r(const char *hostname, int degree){
	int a, b;
	sscanf(hostname, "%d.%d", &a, &b);
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
	sprintf(hostname, "%d.%d", groupnumber, nodenumber);
	return;
}

void program_abort(const char * message) {
	fprintf(stderr, "Error: %s", message);
	exit(1);
}
