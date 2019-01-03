#include "testing_utils.h"

// dot product of two matrices a and b, result saved in matrix c
void dotProduct(float **c, float **a, float **b, int n){

    for(int i = 0; i < n; i++){
        for(int k = 0; k < n; k++){
            for(int j = 0; j < n; j++){
                c[i][j] += a[i][k] * b[k][j];
            }       
        }   
    }   
}

// dot product of two matrices with block optimization
void dotProductBlockOptimized(float **c, float **a, float **b, int n){
    int blocksize = 10;
    double sum;

    for(int j_block = 0; j_block < n; j_block += blocksize)
        for(int k_block = 0; k_block < n; k_block += blocksize)
            for(int i = 0; i < n; i ++)
                for(int j = j_block; j < j_block+blocksize; j ++){
                    sum = 0.f;
                    for(int k = k_block; k < k_block+blocksize; k ++)
                        sum += a[i][k] * b[k][j];
                    c[i][j] += sum;
                }
}

//main function
int main(){
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

    //start();
    dotProduct(c,a,b,N);
    //printf("%llu usecs \n", stop());
    
    //start();
    //dotProductBlockOptimized(c,a,b,N);
    //printf("%llu usecs \n", stop());
    
    return 0;
}
