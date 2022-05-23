#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "header.h"
// #include "mpi.h"

int main(  )
{	
	int d = 3;
	int count  = 10; 
	for (int i = 0; i < d; i++) {
		// Split the data
		size_t amount = round((1.0*count / d));
		if (i == d - 1) amount = count - (d - 1) * amount;

		size_t position = round(i * (1.0*count / d));
		// Send to neighbor
		printf("Position: %ld, amount: %ld\n", position, amount);
	}
	return 0;
}
