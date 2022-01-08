#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <smpi/smpi.h>
#include <math.h>

int main(int argc, char *argv[])
{
	int size, rank;
	struct timeval start, end;
	char hostname[2048];
	int hostname_len;
	int message_size = 1024 * 1024;
	
	gettimeofday(&start,NULL);
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Get_processor_name(hostname,&hostname_len);

	/***********        Parameter    ************/
	int D =	1280000;	// Dataset size (number of samples)
	D = atoi(argv[1]);
	int B = 64; //Minibatch size per node.
	B = atoi(argv[2]);
	float BS = B * size;
	int I = (int) ceil(((float) D)/BS); //1280000/B; //Number of Iteration per epoch
	int E = 1; //Number of epoch;
	E = atoi(argv[3]);
	int totalIter = I * E;
	if (rank == 0) {
		gettimeofday(&end,NULL);
		printf("Params:Data set size\t%d\n",D);
		printf("Params:Batch per node\t%d\n",B);
		printf("Params:Number of node\t%d\n",size);
		printf("Params:Mini Batch size\t%f\n",BS);
		printf("Params:Iteration per epoch\t%d\n",I);
		printf("Params:Number of  epoch\t%d\n",E);
		printf("Params:Number of iteration\t%d\n",totalIter);
	}
	totalIter = 1; // Run only 1 iteration for fast finish...
	/******* Neural Model Declaration ***********/
	/* RESNET50 Deep Neural Network for the sample (image) size 244x244x3 
	Layer		Activation |Yi |	Weight |WTi |		#Operations (FW)
	INPUT		244x244x3			0					0
	CONV7-64/2	112x112x64			7x7x3x64			112x112x64x3x7x7
	POOL2		56x56x64			0					56x56x64x3x3x64
	CONV1-64	56x56x64			1x1x64x64			56x56x64x1x1x64
	CONV3-64	56x56x64			3x3x64x64			56x56x64x3x3x64
	CONV1-256	56x56x256			1x1x64x256			56x56x256x1x1x64
	CONV1-64	56x56x64			1x1x64x64			56x56x64x1x1x256
	CONV3-64	56x56x64			3x3x64x64			56x56x64x3x3x64
	CONV1-256	56x56x256			1x1x64x256			56x56x256x1x1x64
	CONV1-64	56x56x64			1x1x64x64			56x56x64x1x1x256
	CONV3-64	56x56x64			3x3x64x64			56x56x64x3x3x64
	CONV1-256	56x56x256			1x1x64x256			56x56x256x1x1x64
	CONV1-128	28x28x128			1x1x256x128			28x28x128x1x1x256
	CONV3-128	28x28x128			3x3x128x128			28x28x128x3x3x128
	CONV1-512	28x28x512			1x1x128x512			28x28x512x1x1x128
	CONV1-128	28x28x128			1x1x512x128			28x28x128x1x1x512
	CONV3-128	28x28x128			3x3x128x128			28x28x128x3x3x128
	CONV1-512	28x28x512			1x1x128x512			28x28x512x1x1x128
	CONV1-128	28x28x128			1x1x512x128			28x28x128x1x1x512
	CONV3-128	28x28x128			3x3x128x128			28x28x128x3x3x128
	CONV1-512	28x28x512			1x1x128x512			28x28x512x1x1x128
	CONV1-128	28x28x128			1x1x512x128			28x28x128x1x1x512
	CONV3-128	28x28x128			3x3x128x128			28x28x128x3x3x128
	CONV1-512	28x28x512			1x1x128x512			28x28x512x1x1x128
	CONV1-256	14x14x256			1x1x512x256			14x14x256x1x1x512
	CONV3-256	14x14x256			3x3x256x256			14x14x256x3x3x256
	CONV1-1024	14x14x1024			1x1x256x1024		14x14x1024x1x1x256
	CONV1-256	14x14x256			1x1x1024x256		14x14x256x1x1x1024
	CONV3-256	14x14x256			3x3x256x256			14x14x256x3x3x256
	CONV1-1024	14x14x1024			1x1x256x1024		14x14x1024x1x1x256
	CONV1-256	14x14x256			1x1x1024x256		14x14x256x1x1x1024
	CONV3-256	14x14x256			3x3x256x256			14x14x256x3x3x256
	CONV1-1024	14x14x1024			1x1x256x1024		14x14x1024x1x1x256
	CONV1-256	14x14x256			1x1x1024x256		14x14x256x1x1x1024
	CONV3-256	14x14x256			3x3x256x256			14x14x256x3x3x256
	CONV1-1024	14x14x1024			1x1x256x1024		14x14x1024x1x1x256
	CONV1-256	14x14x256			1x1x1024x256		14x14x256x1x1x1024
	CONV3-256	14x14x256			3x3x256x256			14x14x256x3x3x256
	CONV1-1024	14x14x1024			1x1x256x1024		14x14x1024x1x1x256
	CONV1-256	14x14x256			1x1x1024x256		14x14x256x1x1x1024
	CONV3-256	14x14x256			3x3x256x256			14x14x256x3x3x256
	CONV1-1024	14x14x1024			1x1x256x1024		14x14x1024x1x1x256
	CONV1-512	7x7x512				1x1x1024x512		7x7x512x1x1x1024
	CONV3-512	7x7x512				3x3x512x512			7x7x512x3x3x512
	CONV1-2048	7x7x2048			1x1x512x2048		7x7x2048x1x1x512
	CONV1-512	7x7x512				1x1x2048x512		7x7x512x1x1x2048
	CONV3-512	7x7x512				3x3x512x512			7x7x512x3x3x512
	CONV1-2048	7x7x2048			1x1x512x2048		7x7x2048x1x1x512
	CONV1-512	7x7x512				1x1x2048x512		7x7x512x1x1x2048
	CONV3-512	7x7x512				3x3x512x512			7x7x512x3x3x512
	CONV1-2048	7x7x2048			1x1x512x2048		7x7x2048x1x1x512
	POOL2		1x1x1000			0					1x1x1000x7x7x2048
	FC1000		1x1x1000			1000x1000			1000x1000
	Total		9.4M				21.7M				5.66B
	*/  
	
	int i = 0;
	double input = 244*244*3;
	double nw[52][3] = {
		 {112*112*64,7*7*3*64,112*112*64*3*7*7},
		 {56*56*64,0,56*56*64*3*3*64},
		 {56*56*64,1*1*64*64,56*56*64*1*1*64},
		 {56*56*64,3*3*64*64,56*56*64*3*3*64},
		 {56*56*256,1*1*64*256,56*56*256*1*1*64},
		 {56*56*64,1*1*64*64,56*56*64*1*1*256},
		 {56*56*64,3*3*64*64,56*56*64*3*3*64},
		 {56*56*256,1*1*64*256,56*56*256*1*1*64},
		 {56*56*64,1*1*64*64,56*56*64*1*1*256},
		 {56*56*64,3*3*64*64,56*56*64*3*3*64},
		 {56*56*256,1*1*64*256,56*56*256*1*1*64},
		 {28*28*128,1*1*256*128,28*28*128*1*1*256},
		 {28*28*128,3*3*128*128,28*28*128*3*3*128},
		 {28*28*512,1*1*128*512,28*28*512*1*1*128},
		 {28*28*128,1*1*512*128,28*28*128*1*1*512},
		 {28*28*128,3*3*128*128,28*28*128*3*3*128},
		 {28*28*512,1*1*128*512,28*28*512*1*1*128},
		 {28*28*128,1*1*512*128,28*28*128*1*1*512},
		 {28*28*128,3*3*128*128,28*28*128*3*3*128},
		 {28*28*512,1*1*128*512,28*28*512*1*1*128},
		 {28*28*128,1*1*512*128,28*28*128*1*1*512},
		 {28*28*128,3*3*128*128,28*28*128*3*3*128},
		 {28*28*512,1*1*128*512,28*28*512*1*1*128},
		 {14*14*256,1*1*512*256,14*14*256*1*1*512},
		 {14*14*256,3*3*256*256,14*14*256*3*3*256},
		 {14*14*1024,1*1*256*1024,14*14*1024*1*1*256},
		 {14*14*256,1*1*1024*256,14*14*256*1*1*1024},
		 {14*14*256,3*3*256*256,14*14*256*3*3*256},
		 {14*14*1024,1*1*256*1024,14*14*1024*1*1*256},
		 {14*14*256,1*1*1024*256,14*14*256*1*1*1024},
		 {14*14*256,3*3*256*256,14*14*256*3*3*256},
		 {14*14*1024,1*1*256*1024,14*14*1024*1*1*256},
		 {14*14*256,1*1*1024*256,14*14*256*1*1*1024},
		 {14*14*256,3*3*256*256,14*14*256*3*3*256},
		 {14*14*1024,1*1*256*1024,14*14*1024*1*1*256},
		 {14*14*256,1*1*1024*256,14*14*256*1*1*1024},
		 {14*14*256,3*3*256*256,14*14*256*3*3*256},
		 {14*14*1024,1*1*256*1024,14*14*1024*1*1*256},
		 {14*14*256,1*1*1024*256,14*14*256*1*1*1024},
		 {14*14*256,3*3*256*256,14*14*256*3*3*256},
		 {14*14*1024,1*1*256*1024,14*14*1024*1*1*256},
		 {7*7*512,1*1*1024*512,7*7*512*1*1*1024},
		 {7*7*512,3*3*512*512,7*7*512*3*3*512},
		 {7*7*2048,1*1*512*2048,7*7*2048*1*1*512},
		 {7*7*512,1*1*2048*512,7*7*512*1*1*2048},
		 {7*7*512,3*3*512*512,7*7*512*3*3*512},
		 {7*7*2048,1*1*512*2048,7*7*2048*1*1*512},
		 {7*7*512,1*1*2048*512,7*7*512*1*1*2048},
		 {7*7*512,3*3*512*512,7*7*512*3*3*512},
		 {7*7*2048,1*1*512*2048,7*7*2048*1*1*512},
		 {1*1*1000,0,1*1*1000*7*7*2048},
		 {1*1*1000,1000*1000,1000*1000}
	 };
	int L = 52; // Number of Layer
	message_size = 0;
	for (i = 0; i < L; i++){
		message_size = message_size + nw[i][1];
	}	
	
	if (rank == 0) {
		for (i = 0; i < L; i++){
			printf("Layer %d [%f,%f,%.1f]\n",i,nw[i][0],nw[i][1],nw[i][2]);
		}
		printf("Total number of gradient %d\n",message_size);
	}
	float *local_sum = malloc(sizeof(float) * message_size);
	float *global_sum = malloc(sizeof(float) * message_size);
	MPI_Barrier(MPI_COMM_WORLD);
	/****** Training ***********/
 	gettimeofday(&end,NULL);
	if (rank == 0) {
		printf("Start training\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
			start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0);  
	}
		
	int iter;
	for (iter = 0; iter < totalIter; iter++){
		// FW computation
		if (rank == 0) {
			gettimeofday(&end,NULL);
			printf("Start FW\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
				start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0);  
		}
		for (i = 0; i < L; i++){
			SMPI_SAMPLE_FLOPS(nw[i][2]*B) {}
		}
		if (rank == 0) {
			gettimeofday(&end,NULL);
			printf("End FW\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
				start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0); 
			printf("Start BW\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
				start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0);  
		}
		// BW computation
		for (i = L-1; i >= 0; i--){
			SMPI_SAMPLE_FLOPS(nw[i][2]*B) {}
		}
		if (rank == 0) {
			gettimeofday(&end,NULL);
			printf("End BW\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
				start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0); 
			printf("Start Update\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
				start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0);   
		}
		//Activation Gradient calculation and Weight gradient
		for (i = 0; i < L; i++){
			SMPI_SAMPLE_FLOPS(nw[i][1] + nw[i][0]*B) {}
		}
		if (rank == 0) {
			gettimeofday(&end,NULL);
			printf("End Update\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
				start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0); 
			printf("Start Communication\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
				start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0);   
		}
		
		//Weight update:
		MPI_Allreduce(local_sum, global_sum, message_size, MPI_FLOAT, MPI_SUM,MPI_COMM_WORLD);
		if (rank == 0) {
			gettimeofday(&end,NULL);
			printf("End Communication\t%f\n",(end.tv_sec*1000000.0 + end.tv_usec -
				start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0);  
		}		
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		gettimeofday(&end,NULL);
		printf("End training\t %f\n",(end.tv_sec*1000000.0 + end.tv_usec -
			start.tv_sec*1000000.0 - start.tv_usec) / 1000000.0);  
	}
	MPI_Finalize();
	return 0;
}
