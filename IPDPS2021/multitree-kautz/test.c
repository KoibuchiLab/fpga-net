#include <stdio.h>
#include <stdlib.h>
// #include "mpi.h"

int main(  )
{	
	int aa = 272;
	int a = 272 * 16777216;
	unsigned long b = 272 ;
	unsigned long c = 16777216;
	unsigned long d = b *c;
	printf("%ld %d %ld\n", sizeof(size_t), a, d);
	float* kim = (float*)malloc(sizeof(float)*d);
	kim[d -1] = 0.2;
	printf("%ld", aa * c);
	for (size_t i = 0; i < aa * c; i++) {
		kim[i] = i;
	}
	printf("%ld %f\n", d, kim[d - 1]);
	return 0;
}
