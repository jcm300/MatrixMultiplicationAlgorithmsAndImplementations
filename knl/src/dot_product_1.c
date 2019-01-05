#include "testing_utils.h"


void tileMult(float **a, float **b, float **c){
    for(int i = 0; i < BLOCK_SIZE; i ++)
        for(int j = 0; j < BLOCK_SIZE; j ++)
            for(int k = 0; k < BLOCK_SIZE; k ++)
                c[i][j] +=  a[i][k] * b[k][j];
}

// dot product of two matrices with block optimization
void dotProductBlockOptimized(float ** __restrict__ c, float ** __restrict__ a, float ** __restrict__ b, int n){
    
    #pragma omp parallel for
    for(int j_block = 0; j_block < n; j_block += BLOCK_SIZE)
        for(int k_block = 0; k_block < n; k_block += BLOCK_SIZE)
            for(int i = 0; i < n; i ++)
                for(int k = k_block; k < k_block+BLOCK_SIZE; k ++){
                    #pragma omp parallel for num_threads(64)
                    for(int j = j_block; j < j_block+BLOCK_SIZE; j ++)
                        c[i][j] +=  a[i][k] * b[k][j];
                }

}

//main function
int main(){
    unsigned seed=0;
    float __attribute__((aligned(32))) **a = (float**)malloc(sizeof(float*)*N);
    float __attribute__((aligned(32))) **b = (float**)malloc(sizeof(float*)*N);
    float __attribute__((aligned(32))) **c = (float**)malloc(sizeof(float*)*N);

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

    start();
    dotProductBlockOptimized(c,a,b,N);
    printf("%llu usecs \n", stop());

    return 0;
}
