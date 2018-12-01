#include <stdlib.h>

#define N 50

// dot product of two matrices a and b, result saved in matrix c
void dotProduct(float *c, float *a, float *b, int n){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            *(c+i*n+j) = 0;
            for(int k = 0; k < n; k++){
                *(c+i*n+j) += *(a+i*n+k) * *(b+k*n+j);
            }       
        }   
    }   
}

//main function
int main(){
    unsigned seed;
    float a[N][N], b[N][N], c[N][N];

    srand(seed);

    //build matrix A with random values
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            a[i][j] = rand();

    //build matrix B with all elements equals to 1
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            b[i][j] = 1;

    dotProduct(&(c[0][0]),&(a[0][0]),&(b[0][0]),N);

    return 0;
}
