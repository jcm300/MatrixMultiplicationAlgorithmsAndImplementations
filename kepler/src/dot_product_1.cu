#include <iostream>
#include <cstdlib>
#include <sys/time.h>

#define N 32	// Only powers of 2 to simplify the code
#define BLOCK_SIZE 8
#define NUM_BLOCKS N
#define NUM_THREADS_PER_BLOCK N
#define NUM_BLOCKS_TILED (N*N)/(BLOCK_SIZE*BLOCK_SIZE)
#define NUM_THREADS_PER_BLOCK_TILED BLOCK_SIZE*BLOCK_SIZE
#define TIME_RESOLUTION 1000000

using namespace std;

long long unsigned initial_time;
struct timeval t;

void printResults (long long unsigned tt) {
	cout << tt << endl;
}

void start (void) {
	gettimeofday(&t, NULL);
	initial_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;
}

long long unsigned stop (void) {
	gettimeofday(&t, NULL);
	long long unsigned final_time = t.tv_sec * TIME_RESOLUTION + t.tv_usec;

	return final_time - initial_time;
}

__global__ void matrixKernel (float *dev_m1, float *dev_m2, float *dev_result) {
    
    *(dev_result+blockIdx.x*N+threadIdx.x)=0;
    
	for(unsigned i=0; i < N; i++)
		*(dev_result+blockIdx.x*N+threadIdx.x) += *(dev_m1+blockIdx.x*N+i) * *(dev_m2+i*N+threadIdx.x);
}

void gpuMatrixMult (float *m1, float *m2, float *result) {
	float *dev_m1, *dev_m2, *dev_result;

	cudaMalloc((void**) &dev_m1,N * N * sizeof(float));
	cudaMalloc((void**) &dev_m2,N * N * sizeof(float));
	cudaMalloc((void**) &dev_result, N * N * sizeof(float));
    
	//startTime
	start();
	
    cudaMemcpy(dev_m1, m1, N * N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_m2, m2, N * N * sizeof(float), cudaMemcpyHostToDevice);

	matrixKernel <<< NUM_THREADS_PER_BLOCK, NUM_BLOCKS >>>(dev_m1, dev_m2, dev_result);
    
	// copy the output to the host
    cudaMemcpy(result, dev_result, N * N * sizeof(float), cudaMemcpyDeviceToHost);
    
	//stopTime
	printResults(stop());

	// free the device memory
	cudaFree(dev_m1);
	cudaFree(dev_m2);
	cudaFree(dev_result);
}

/*
__global__ void tiledMatrixKernel (float *dev_m1, float *dev_m2, float *dev_result) {

	__shared__ float temp1 [BLOCK_SIZE][BLOCK_SIZE];
	__shared__ float temp2 [BLOCK_SIZE][BLOCK_SIZE];
	int xIn = threadIdx.x/BLOCK_SIZE;
	int yIn = threadIdx.x%BLOCK_SIZE;
	int xB = (((blockIdx.x*BLOCK_SIZE) / N) * BLOCK_SIZE) + xIn;
	int yB = (((blockIdx.x*BLOCK_SIZE) % N) * BLOCK_SIZE) + yIn;

	temp1[xIn][yIn]=*(dev_m1+xB*N+yB);
	temp2[xIn][yIn]=*(dev_m2+xB*N+yB);

	__syncthreads();

	for(unsigned i=0; i < BLOCK_SIZE; i++)
		*(dev_result+xB*N+yB) += temp1[xIn][i]*temp2[i][yIn];
}

void gpuTiledMatrixMult (float *m1, float *m2, float *result) {
	float *dev_m1, *dev_m2, *dev_result;

	cudaMalloc((void**) &dev_m1,N * N * sizeof(float));
	cudaMalloc((void**) &dev_m2,N * N * sizeof(float));
	cudaMalloc((void**) &dev_result, N * N * sizeof(float));
    
    start();
	//startKernelTime();

    cudaMemcpy(dev_m1, m1, N * N * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_m2, m2, N * N * sizeof(float), cudaMemcpyHostToDevice);

	tiledMatrixKernel <<< NUM_THREADS_PER_BLOCK_TILED, NUM_BLOCKS_TILED >>>(dev_m1, dev_m2, dev_result);

	// copy the output to the host
    cudaMemcpy(result, dev_result, N * N * sizeof(float), cudaMemcpyDeviceToHost);

	//stopKernelTime();
	printResults(stop());
    
	// free the device memory
	cudaFree(dev_m1);
	cudaFree(dev_m2);
	cudaFree(dev_result);
}
*/

int main (int argc, char *argv[]) {

    unsigned seed=0;
    float *a = (float*)malloc(sizeof(float)*N*N);
    float *b = (float*)malloc(sizeof(float)*N*N);
    float *c = (float*)malloc(sizeof(float)*N*N);
    
    srand(seed);
    
    //build matrix A with random values and C initilized with 0's
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            *(a+i*N+j) = rand();
            *(c+i*N+j) = 0;
        }   
    }

    //build matrix B with all elements equals to 1
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
            *(b+i*N+j) = 1;
    }

	gpuMatrixMult(a,b,c);

	//gpuTiledMatrixMult(a,b,c);

	return 0;
}
