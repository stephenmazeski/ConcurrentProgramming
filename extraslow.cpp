#include <iostream>
#include <bits/stdc++.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define DEBUG

void get_walltime(double* wcTime) {

     struct timeval tp;

     gettimeofday(&tp, NULL);

     *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);

}

void dec2bin(long int decimalValue, int *binaryArray, int numBits)
{
	int bp = numBits;
	long int N = decimalValue;

	/*for (int x = 0; x < numBits; ++x)
		binaryArray[x] = 0;*/
	//eliminate for loop
	
	binaryArray[bp];
	memset(binaryArray, 0, sizeof(binaryArray));


	//int newvar = bp -1.0; this variable didnt work
	

	long int newvar2 = pow(2.0,bp);
	// this variable is crucial. Took speed from 45.8s to 3.95s.
	// saves from calculating pow(2.0,bp) with every iteration...
	/*while(N > 0)
	{
		while(N < (pow(2.0,bp)))
		{
			bp = bp-1.0;
		};
		binaryArray[(int)bp] = 1;
		N = N - newvar2;
	}*/
	// Also very crucial step. This saves the iterations from running
	// through two while loops concurrently.

	if ( N>0) goto case_a;
	if (N< (pow(2.0,bp))&& N>0) goto case_b;
	case_a: bp = bp-1.0;
	case_b: N - N - newvar2;goto end;
	end:
;
}

// print the contents of an array
 void printArray(int *array, int numBits)
 {	
		//q = numBits - 1;
		for (int ii = numBits-1; ii>= 0; ii--)
		printf("%i ", array[numBits - ii -1]);
		printf("\n");
 }	
int main(int argc, char **argv)
	{
	int binArray[32];
 	int decimal;
	double d_S1, d_E1;
	int i_R = 10000000;
	srandom(716);
	get_walltime(&d_S1);
	for (uint32_t ii = 0; ii <  i_R; ++ii)
	{
		decimal = random();
		dec2bin(decimal, binArray, 32);
	}
	get_walltime(&d_E1);
	#ifdef DEBUG
	decimal = random();
	dec2bin(decimal, binArray, 32);
	#endif
	printf("Time dec2bin: %f\n", d_E1-d_S1);
	return 0;
	}
//end
