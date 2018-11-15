/**
1) (10 points) Write a serial implementation to calculate the standard-deviation of an array of randomly generated floating point values. Then thread it using std::threads as well as OpenMP (yes, two implementations!). Compare the performance results (over both N and P). Estimate the overhead costs for both parallelization methods.

2) (10 points) Add a capability to the OpenMP implementation to return a list (values and indices) of all values over a sigma value specified by the user.

3) Optional (Extra 5 points) Add the same capability to the std::threads implementation.
**/

#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <cmath>
#include <sys/time.h>
#include <string>
using namespace std;

double sd(double *A, long int n){
long int i;
double runsum;

for (i=0;i<n;i++){
    runsum+=A[i];
    }

double average=runsum/n;
runsum=0;

for (i=0;i<n;i++){
    runsum+=(A[i]-average)*(A[i]-average);
    }

return sqrt(runsum/(n-1));
}

double sd2(double *A, long int n){
long int i;
double runsum=0, average;
omp_set_num_threads(6);
#pragma omp  parallel for private(i) reduction(+: runsum)
for (i=0;i<n;i++){
    runsum+=A[i];
    }

average=runsum/n;
runsum=0;

#pragma omp parallel for private(i) reduction(+: runsum)
for (i=0;i<n;i++){
    runsum+=(A[i]-average)*(A[i]-average);
    }

return sqrt(runsum/(n-1));
}

double sd3(double sigma, long int n, double threads){
long int i, j;
double runsum=0, average;
double wholeassmean2, dist_val;

omp_set_num_threads(threads);
#pragma omp parallel for private(i) reduction(+: wholeassmean2)
for (i=0;i<20;i++){
        wholeassmean2+=i*(1/20);
        }
dist_val=20*((sigma*sigma)+(wholeassmean2*wholeassmean2));

for(i = 0; i <= dist_val; i++){
        runsum+=(i*i);
        if (i != 0){
        printf("%i, index %i\n%i, index %i\n", i, (2*i) - 1,(0-i),(2*i+1) -1);
        }
        else{
        printf("%i, index %i\n", i, 0);
        }
        if(runsum >= dist_val){
        return runsum;
        }
}
}


void get_walltime(double* wcTime) {
     struct timeval tp;
     gettimeofday(&tp, NULL);
     *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(){
long int n=10000000;
double ans,ans2,ans3,S,E,S2,E2,S3,E3;
double  *A;
A=new double [n];
long int i;
double sigma;
double threads;

cout << "Sigma: ";
cin >> sigma;

cout << "Threads: ";
cin >> threads;

for (i=0;i<n;i++){
    A[i]=rand()%20;
    }

get_walltime(&S);
ans=sd(A,n);
get_walltime(&E);

get_walltime(&S2);
ans2=sd2(A,n);
get_walltime(&E2);

get_walltime(&S3);
ans3=sd3(sigma, n, threads);
get_walltime(&E3);

printf("\nTime No OMP %f \n", E-S);
printf( "Standard Deviation No OMP %lf \n\n" , ans);
printf("Time OMP %f \n", E2-S2);
printf( "Standard Deviation OMP %lf \n\n" , ans2);
printf("Time third  %f \n", E3-S3);
printf("Total Sum %lf \n\n" , ans3);
return 0;
}
