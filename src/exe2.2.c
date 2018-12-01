#include <stdlib.h>

#define N 50

// dot product of two matrices a and b, result saved in matrix c
void dotProduct(float **c, float **a, float **b, int n){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            c[i][j] = 0;
            for(int k = 0; k < n; k++){
                c[i][j] += a[i][k] * b[k][j];
            }       
        }   
    }   
}

//main function
int main(){
    unsigned seed;
    float **a = (float**)malloc(sizeof(float)*N*N);
    float **b = (float**)malloc(sizeof(float)*N*N);
    float **c = (float**)malloc(sizeof(float)*N*N);

    srand(seed);

    //build matrix A with random values
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            a[i][j] = rand();

    //build matrix B with all elements equals to 1
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            b[i][j] = 1;

    dotProduct(c,a,b,N);

    return 0;
}
