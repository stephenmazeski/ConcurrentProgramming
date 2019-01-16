#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#define USE_MPI 1
#if USE_MPI
#include <mpi.h>
#endif
#define SEND_B 0 // tag for mpi communication of matrix B
static double timer() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return ((double) (tp.tv_sec) + 1e-6 * tp.tv_usec);
	}
int main(int argc, char **argv) {
	int rank, processes;

	#if USE_MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int nextProc = (rank+1) % processes;
	int prevProc = (rank+ processes -1) % processes;
	#else
	rank = 0;
	processes = 1;
	#endif
	if (argc != 3) {
	if (rank == 0) {
		fprintf(stderr, "Welcome to Conway's Life done on MPI\n\n\n");}//printf("TEST");
	}
	//printf("TEST");
	int m = 1000;//printf("TEST");
	assert(m > 0);//printf("TEST assert 1");
/*I began to run into issues here and ran through a series of tests. The main issue was my lack of knowledge with the assert call. I had not allocated any value to the n value itself and bugs began to arise.*/
	assert(m < 1000000);//printf("TEST assert2");

//	printf("TEST 1");
	m = (m/ processes) * processes;
	int k = (m/ processes);
	if (rank == 0) {
	fprintf(stderr, "m: %d, k: %d\n", m, k);
	fprintf(stderr, "Requires %3.6lf MB of memory foreach task\n", ((3*4.0*k)*m/1e6));
	}
	float *A, *B, *C;
	A = malloc(k * m * sizeof(float));
	assert(A != 0);
	B =  malloc(k * m * sizeof(float));
	assert(B != 0);
	C =  malloc(k * m * sizeof(float));
	assert(C != 0);
	int i, j,t;
	assert((m*k/2 + m/2 + 3) < k*m);

	//place data within mxm matrix
	A[m*k/2 + m/2 + 0] = 1;
	A[m*k/2 + m/2 + 1] = 1;
	B[m*k/2 + m/2 + 2] = 1;
	B[m*k/2 + m/2 + 3] = 1;
	double start= 0.0, end = 0.0;
	
	//printf("Hello!!");
	for (i=0; i<k; i++) {
	for (j=0; j<m; j++) {
	A[i*m+j] = (rank+1);
	B[i*m+j] = 1;
	C[i*m+j] = 0;
	}
	}
	#if USE_MPI
	MPI_Barrier(MPI_COMM_WORLD);
	#endif
	start = 0.0;
	if (rank == 0)
	start = timer();
	#if USE_MPI
	float* tempB; 
	MPI_Status status;
	int g,h, x,u;
	tempB = malloc(k * m * sizeof(float));
	assert(tempB != 0);
	for (u = 0; u < processes; ++u) {
	
	int aOffset = (u + rank) % processes;
	for(g = 0; g < m; ++g) {
	for(x = 0; x < m; ++x) {
	int cIndex = g * m + x;
	for(u = 0; u < k; ++u) {
	int aIndex = g * m + aOffset + u;
	int bIndex = u * m + x;
	C[cIndex] += A[aIndex] * B[bIndex];
	}
}
}
	//printf("HELLO2");
	MPI_Sendrecv(B, k * m, MPI_FLOAT, prevProc, SEND_B,
	tempB, k * m, MPI_FLOAT, nextProc, SEND_B,
	MPI_COMM_WORLD, &status);
	MPI_Barrier(MPI_COMM_WORLD);
	int q;
	for(q = 0; q < k * m; q++) {
	B[q] = tempB[q];
	//printf("BARRIER TEST");
	}
	}
	free(tempB);
	#else
	int s;
	for (i=0; i<k; i++) {printf("HI");
	for (j=0; j<m; j++) {
	float c_ij = 0;
	for (s=0; s<m; s++) {
	c_ij += A[i*m+k]*B[k*m+j];
	}
	C[i*m+j] = c_ij;
	}
	}
	#endif
	if (rank == 0)
	end = timer();
	
	if (rank == 0) {
	fprintf(stderr, "Time (in seconds) %3.3lf s.\n", (end-start));
	fprintf(stderr, "Performance: %3.3lf GFlop/s\n", (2.0*m*m)*m/((end-start)*1e9));
	}
	//#if USE_MPI
	//MPI_ABORT(MPI_COMM_WORLD,1);
	//#endif
	free(A); free(B); free(C);

	#if USE_MPI
	MPI_Finalize();
	#endif
	return 0;}
