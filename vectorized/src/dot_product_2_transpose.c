#include "testing_utils.h"

/*
 * Perform dot product of two matrices
*/

// Inplace transpose
void transpose(float **m, int n){
    float temp;
    for(int i=0; i<n; i++)
        for(int j=i; j<n; j++){
            temp = m[i][j];
            m[i][j] = m[j][i];
            m[j][i] = temp;
        }
}


// dot product of two matrices with block optimization
void dotProductBlockOptimized(float **c, float **a, float **b, int n){
    for(int j_block = 0; j_block < n; j_block += BLOCK_SIZE)
        for(int k_block = 0; k_block < n; k_block += BLOCK_SIZE)
            for(int j = j_block; j < j_block+BLOCK_SIZE; j ++)
                for(int k = k_block; k < k_block+BLOCK_SIZE; k ++)
                    for(int i = 0; i < n; i ++)
                        c[i][j] += a[k][i] * b[j][k];
}

void printMatrix(float **m, int n){
    for(int i=0; i<n; i++){
        for(int j=0;j<n;j++){
            printf("%.5f ", m[i][j]);
            m[i][j] = 0.f;
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    unsigned seed=0;
    float **a = (float**)malloc(sizeof(float*)*N); 
    float **b = (float**)malloc(sizeof(float*)*N);
    float **c = (float**)malloc(sizeof(float*)*N);

    srand(seed);

    for(int i = 0; i < N; i ++){
        a[i] = (float*)malloc(sizeof(float)*N);
        c[i] = (float*)calloc(N, sizeof(float));
        for(int j = 0; j < N; j ++)
            a[i][j] = rand();
    }

    for(int i = 0; i < N; i ++){
        b[i] = (float*)malloc(sizeof(float)*N);
        for(int j = 0; j < N; j ++)
            b[i][j] = 1;
    }

    transpose(a,N);
    transpose(b,N);

    start();
    dotProductBlockOptimized(c,a,b,N);
    printf("%llu usecs \n", stop());
}
