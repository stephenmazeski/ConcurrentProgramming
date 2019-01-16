#include<iostream>
using namespace std;
#include <sys/time.h>
#include <fstream>
#include <stdlib.h>
#include<stdio.h>


void get_walltime(double* wcTime)
{

        struct timeval tp;
        gettimeofday(&tp, NULL);
        *wcTime = (double)((tp.tv_sec + tp.tv_usec) / 1000000.0);
}

int main(int argc, char **argv)
{	
//	int max = 10000000;
	double S,E, MFLOPS;
	int i,j,k;
	int R=5;
	int N;
	std::cout << " Setting environment..." << endl;
       for(int N = 10; N<100; N+=10){
       // cout << "Beginning Trials..." << endl;
        double *A, *B, *C, *D;
        A = new double[N];
 	B = new double[N];
        C = new double[N];
        D = new double[N];
        for(i = 1; i<N+1; i++){
                A[i] = 0.0;
                B[i] = 1.0;
                C[i] = 2.0;
                D[i] = 3.0;
           }
        get_walltime(&S);
 	for(j=1;j<R+1;j++){
                for(i=1;i<N+1;i++){
                       A[i] = B[i] + C[i] * D[i];}
        if(A[2]<0)
               {break;}
        }
        get_walltime(&E);
        MFLOPS = ((R*N*2.0)/((E-S)*1000000.0));
       cout << "\n R-value:  " << R << "\n";
       cout << "\n N-value:  " << N << "\n";
       cout << "\n MFLOPS:  " << MFLOPS << "\n";
	ofstream myfile;
	myfile.open("aci-imallmain.txt", ofstream::app);
	myfile << R << "\t" << N << "\t" << MFLOPS << "\n";
	
	}
}
