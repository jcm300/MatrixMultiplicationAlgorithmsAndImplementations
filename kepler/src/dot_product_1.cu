#include <iostream>
#include <cstdlib>
#include <sys/time.h>

#define N 1024	// Only powers of 2 to simplify the code
#define BLOCK_SIZE 32
#define NUM_BLOCKS N
#define NUM_THREADS_PER_BLOCK N
#define NUM_BLOCKS_TILED (N*N)/(BLOCK_SIZE*BLOCK_SIZE)
#define NUM_THREADS_PER_BLOCK_TILED BLOCK_SIZE*BLOCK_SIZE
#define TIME_RESOLUTION 1000000

using namespace std;

long long unsigned initial_time;
struct timeval t;

void printResults (long long unsigned tt) {
	cout << "Execution time: " << tt << " usecs" << endl;
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

__global__ void matrixKernel (float **dev_m1, float **dev_m2, float **dev_result) {

    dev_result[blockIdx.x][threadIdx.x]=0;

	for(unsigned i=0; i < N; i++)
		dev_result[blockIdx.x][threadIdx.x] += dev_m1[blockIdx.x][i]*dev_m2[i][threadIdx.x];
}

void gpuMatrixMult (float **m1, float **m2, float **result) {
	float **dev_m1, **dev_m2, **dev_result;

	cudaMalloc((void***) &dev_m1, N * sizeof(float *));
	cudaMalloc((void***) &dev_m2, N * sizeof(float *));
	cudaMalloc((void***) &dev_result, N * sizeof(float *));

	for(unsigned i=0; i < N; i++){
		cudaMalloc((void**) &(dev_m1[i]), N * sizeof(float));
		cudaMalloc((void**) &(dev_m2[i]), N * sizeof(float));
		cudaMalloc((void**) &(dev_result[i]), N * sizeof(float));
	}
    
	//startTime
	start();

	cudaMemcpy(dev_m1, m1, N * sizeof(float *), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_m2, m2, N * sizeof(float *), cudaMemcpyHostToDevice);
	
	for(unsigned i=0; i < N; i++){
		cudaMemcpy(dev_m1[i], m1[i], N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dev_m2[i], m2[i], N * sizeof(float), cudaMemcpyHostToDevice);
	}

	matrixKernel <<< NUM_THREADS_PER_BLOCK, NUM_BLOCKS >>>(dev_m1, dev_m2, dev_result);

	// copy the output to the host
	cudaMemcpy(result, dev_result, N * sizeof(float *), cudaMemcpyDeviceToHost);
	for(unsigned i=0; i < N; i++)
		cudaMemcpy(result[i], dev_result[i], N * sizeof(float), cudaMemcpyDeviceToHost);

	//stopTime
    long long unsigned time = stop();
	printResults(time);

	// free the device memory
	cudaFree(dev_m1);
	cudaFree(dev_m2);
	cudaFree(dev_result);
}


__global__ void tiledMatrixKernel (float **dev_m1, float **dev_m2, float **dev_result) {

	__shared__ float temp1 [BLOCK_SIZE][BLOCK_SIZE];
	__shared__ float temp2 [BLOCK_SIZE][BLOCK_SIZE];
	int xIn = threadIdx.x/BLOCK_SIZE;
	int yIn = threadIdx.x%BLOCK_SIZE;
	int xB = (((blockIdx.x*BLOCK_SIZE) / N) * BLOCK_SIZE) + xIn;
	int yB = (((blockIdx.x*BLOCK_SIZE) % N) * BLOCK_SIZE) + yIn;

	temp1[xIn][yIn]=dev_m1[xB][yB];
	temp2[xIn][yIn]=dev_m2[xB][yB];

	__syncthreads();

	for(unsigned i=0; i < BLOCK_SIZE; i++)
		dev_result[xB][yB] += temp1[xIn][i]*temp2[i][yIn];
}

void gpuTiledMatrixMult (float **m1, float **m2, float **result) {
	float **dev_m1, **dev_m2, **dev_result;

	cudaMalloc((void***) &dev_m1, N * sizeof(float *));
	cudaMalloc((void***) &dev_m2, N * sizeof(float *));
	cudaMalloc((void***) &dev_result, N * sizeof(float *));

	for(unsigned i=0; i < N; i++){
		cudaMalloc((void**) &(dev_m1[i]), N * sizeof(float));
		cudaMalloc((void**) &(dev_m2[i]), N * sizeof(float));
		cudaMalloc((void**) &(dev_result[i]), N * sizeof(float));
	}
    
    start();
	//startKernelTime();

	cudaMemcpy(dev_m1, m1, N * sizeof(float *), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_m2, m2, N * sizeof(float *), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_result, result, N * sizeof(float *), cudaMemcpyHostToDevice);
	
	for(unsigned i=0; i < N; i++){
		cudaMemcpy(dev_m1[i], m1[i], N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dev_m2[i], m2[i], N * sizeof(float), cudaMemcpyHostToDevice);
		cudaMemcpy(dev_result[i], result[i], N * sizeof(float), cudaMemcpyHostToDevice);
	}

	tiledMatrixKernel <<< NUM_THREADS_PER_BLOCK_TILED, NUM_BLOCKS_TILED >>>(dev_m1, dev_m2, dev_result);

	// copy the output to the host
	cudaMemcpy(result, dev_result, N * sizeof(float *), cudaMemcpyDeviceToHost);
	for(unsigned i=0; i < N; i++)
		cudaMemcpy(result[i], dev_result[i], N * sizeof(float), cudaMemcpyDeviceToHost);

	//stopKernelTime();
    long long unsigned time = stop();
	printResults(time);

	// free the device memory
	cudaFree(dev_m1);
	cudaFree(dev_m2);
	cudaFree(dev_result);
}

int main (int argc, char *argv[]) {

    unsigned seed=0;
    float **a = (float**)malloc(sizeof(float*)*N);
    float **b = (float**)malloc(sizeof(float*)*N);
    float **c = (float**)malloc(sizeof(float*)*N);

    srand(seed);

    //build matrix A with random values and C initilized with 0's
    for(int i = 0; i < N; i++){
        c[i] = (float*) malloc(sizeof(float)*N);
        a[i] = (float*) malloc(sizeof(float)*N);
        for(int j = 0; j < N; j++){
            a[i][j] = rand();
            c[i][j] = 0;
        }   
    }

    //build matrix B with all elements equals to 1
    for(int i = 0; i < N; i++){
        b[i] = (float*) malloc(sizeof(float)*N);
        for(int j = 0; j < N; j++)
            b[i][j] = 1;
    }

	gpuMatrixMult(a,b,c);

	for (unsigned i = 0; i < N; i++)
		for (unsigned j = 0; j < N; j++)
			c[i][j] = 0;

	gpuTiledMatrixMult(a,b,c);

	return 0;
}
