#include "testing_utils.h"

int Events[NUM_EVENTS]={PAPI_LD_INS, PAPI_SR_INS};
//int Events[NUM_EVENTS]={PAPI_TOT_INS,PAPI_L3_TCM};
int retval;
long long values[NUM_EVENTS];

// dot product of two matrices a and b, result saved in matrix c
void dotProduct(float **c, float **a, float **b, int n){
    
    PAPI_start_counters(Events,NUM_EVENTS);

    for(int i = 0; i < n; i++){
        for(int k = 0; k < n; k++){
            for(int j = 0; j < n; j++){
                c[i][j] += a[i][k] * b[k][j];
            }       
        }   
    }

    retval = PAPI_stop_counters(values,NUM_EVENTS);
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

    start();
    dotProduct(c,a,b,N);
    printf("%llu\n", stop());

    for(int i=0; i<NUM_EVENTS; i++){
        printf("%lld\n",values[i]);
    }

    return 0;
}
