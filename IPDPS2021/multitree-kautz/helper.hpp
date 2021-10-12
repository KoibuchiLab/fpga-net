/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 11:49:20
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-12 01:36:26
 * @ Description:
 */

#ifndef IPDPS2021_MULTITREE_KAUTZ_HELPER_HPP_
#define IPDPS2021_MULTITREE_KAUTZ_HELPER_HPP_
#include <iostream>
#include <cstdlib>

struct Int3 {
	int dst;
	int src;
	int sendidx;
	int recvidx;
};

#define MULTITREE	0
#define CONGESTION	1
#define COMBINE		2
int h2r(const char *hostname, int degree);
int hidx2r(const int a, const int b, int degree);
void r2h(int rank, int degree, int &a, int &b);
void r2h_r(int rank, int degree, char *hostname);
void program_abort(const char * message);
#endif /*IPDPS2021_MULTITREE_KAUTZ_HELPER_HPP_*/