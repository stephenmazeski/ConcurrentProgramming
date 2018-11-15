#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <cmath>
#include <sys/time.h>
#include <string>
using namespace std;
//standard function for getting time value
void get_walltime(double* wcTime) {
     struct timeval tp;
     gettimeofday(&tp, NULL);
     *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);}
// insertion sorthing algorithm
// This algorithm is not parallelized
double insertionsort(double *A, int n)
	{
		int tt, bb, cc;
		for(tt = 1; tt < n; tt++)
		{

		bb = tt -1;

		while(cc >=0 && A[cc] > bb){
			A[cc + 1] = A[cc];
			cc = cc-1;}
			A[cc+1] = bb;
		}
	
	}
//insertion algorithm
//this algorithm is parallelized
double coolerinsertionsort(double *A, int n)
	{
	int AB;
	omp_set_num_threads(4);
	#pragma omp parallel for private(AB)
	for(AB = 1; AB < n; AB++)
	{
		int B, C;
		B = AB - 1;
//	#pragma omp for schedule(static) nowait	
		while(C>=0 && A[C] > B){
			A[C+1] = A[C];
			C = C-1;
			}
			A[C+1] =B;
		}
	}
		
int main(){
// variables necessary for time along with creating random array
	long int n=10000000;
	double *A;
	double S, E, S2, E2;
	A = new double[n];
	long int i;
// random array populating algorithm

	
	for(i = 0; i<n; i++){
		A[i] = rand()%20;}


	get_walltime(&S);
	insertionsort(A, n);
	get_walltime(&E);
//show results for test
	cout << "Slow sorting algorithm time: " << E-S << endl;

	get_walltime(&S2);
	coolerinsertionsort(A,n);
	get_walltime(&E2);
	cout << "Fast and cool sorting algorithm time: " << E2-S2 << endl;

}
