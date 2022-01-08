#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"
// #include "mpi.h"

int main(  )
{	
	int arraysize = 1231231;
	float *kimarray = (float*)malloc(sizeof(float)*arraysize);
	
	double startime = clock();
	for (int i = 0; i < arraysize; i++){
		kimarray[i] = 0;
	}
	double endtime = (clock() - startime) / 1000000;
	printf("%lf ", endtime);
	startime = clock();
	memset(kimarray, 0.0f, sizeof(float) * arraysize);
	endtime = (clock() - startime) / 1000000;
	printf("%lf ", endtime);
	printf("\n");

	int a = 10, b = 100;
	printf("%d + %d = %d, %d - %d = %d\n", a, b, sum(a, b), a, b, sub(a, b));
	return 0;
}
