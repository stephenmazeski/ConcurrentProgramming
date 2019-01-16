#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 1000000
#define B 1024

__global__ void prescan( float *g_idata, float *INCR, int n);

void scanCPU(float *f_out, float *f_in, int i_n);

double myDiffTime(struct timeval &start, struct timeval &end)
{
        double d_start, d_end;
        d_start = (double)(start.tv_sec + start.tv_usec/1000000.0);
        d_end = (double)(end.tv_sec + end.tv_usec/1000000.0);
        return (d_end - d_start);
}

int main(int argc, char **argv)
{
        float a[N], c[N], g[N];
	
        timeval start, end;
	
        float *dev_a, *dev_g, INCRR[B];
        float size = N*sizeof(float);

//	cudaMallocManaged(&a, N*sizeof(float));
        double d_gpuTime, d_cpuTime;	           
	cudaHostAlloc(&dev_a, size, cudaHostAllocDefault);
	cudaHostAlloc(&dev_g, size, cudaHostAllocDefault);

	dev_a = (float *)malloc(size);
        dev_g = (float *)malloc(size);
	cudaMalloc((void **)&dev_a, size);
        cudaMalloc((void **)&dev_g, size);
	for (int i = 0; i < N; i++)
        {
       
//	a[i] = (float)(rand() % 1000000)/1000.0;
	a[i] = i+1;
        }
	int BATCH = (N/B);
	gettimeofday(&start,NULL);

        cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice); 
        prescan <<< BATCH, B>>> ( dev_a ,INCRR , N); 
	cudaDeviceSynchronize();
        cudaMemcpy(g, dev_g, size, cudaMemcpyDeviceToHost);
        gettimeofday(&end, NULL);
	
        d_gpuTime = myDiffTime(start, end);

        gettimeofday(&start, NULL);
        scanCPU(c, a, N);

        gettimeofday(&end, NULL);
        d_cpuTime = myDiffTime(start, end);


        cudaFree(dev_a); cudaFree(dev_g);

//      for (int i = 0; i < N; i++)
 //     {
  //            printf("c[%i] = %0.3f, g[%i] = %0.3f\n", i, c[i], i, g[i]);
   //  }
        printf("GPU Time for scan size %i: %f\n", N, d_gpuTime);
        printf("CPU Time for scan size %i: %f\n", N, d_cpuTime);
}

__global__ void prescan( float *g_idata, float *INCR, int n)
{
        extern  __shared__  float temp[], g_odata[], SUMS[];
	// allocated on invocation 
	int thid = threadIdx.x + (blockIdx.x * blockDim.x); 
	int offset = 1; 
    	temp[2*thid]   = g_idata[2*thid]; 
	// load input into shared memory 
    	temp[2*thid+1] = g_idata[2*thid+1]; 
	for (int d = B>>1; d > 0; d >>= 1) 
	// build sum in place up the tree 
    	{ 
        	__syncthreads(); 
		if (thid < d)    
        	{ 
			int ai = offset*(2*thid+1)-1; 
			int bi = offset*(2*thid+2)-1; 
		    	temp[bi] += temp[ai];   
		     
  		} 
        	offset *= 2; 
	for(int d =B>>1; d>0; d>>=1){
	//build up sums
	__syncthreads();
	if (thid < d)
              {
                       int ai = offset*(2*thid+1)-1;
                       int bi = offset*(2*thid+2)-1;
			}

    	} 

	if (SUMS && thid == 0) 
	{ 
		SUMS[B] = temp[2*B-1];
		temp[n - 1] = 0; 
	} 

	// clear the last element 
	for (int d = 1; d < n; d *= 2) 
	// traverse down tree & build scan 
    	{ 
        	offset >>= 1; 
        	__syncthreads(); 
		if (thid < d) 
        	{ 
			int ai = offset*(2*thid+1)-1; 
			int bi = offset*(2*thid+2)-1; 
			float t   = SUMS[ai]; 
            		SUMS[ai]  = SUMS[bi]; 
           		SUMS[bi] += INCR[bi]; 
      	} 
    	} 
    	__syncthreads(); 
    	temp[2*thid]   = INCR[2*thid]; 
	// write results to device memory 
    //	g_odata[2*thid+1] = temp[2*thid+1]
//	g_odata[2*thid+1] = INCR[thid];  
}
}
void scanCPU(float *f_out, float *f_in, int i_n)
{
        f_out[0] = 0;
        for (int i = 1; i < i_n; i++)
                f_out[i] = f_out[i-1] + f_in[i-1];

}


