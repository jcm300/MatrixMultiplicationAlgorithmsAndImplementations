#include "testing_utils.h"

int Events[NUM_EVENTS]={PAPI_L1_LDM,PAPI_L1_DCM};
//int Events[NUM_EVENTS]={PAPI_L2_STM,PAPI_L2_DCM};
//int Events[NUM_EVENTS]={PAPI_L3_DCR,PAPI_L3_TCM};
//int Events[NUM_EVENTS]={PAPI_L3_ICA,PAPI_L3_ICR};
int retval;
long long values[NUM_EVENTS];

void dotProduct(float **c, float **a, float **b, int n){
    
    PAPI_start_counters(Events,NUM_EVENTS);

    for(int j=0; j < n; j++)
        for(int k=0; k < n; k++)
            for(int i=0; i < n; i++)
                c[i][j] += a[i][k] * b[k][j]; 

    retval = PAPI_stop_counters(values,NUM_EVENTS);
}

void printMatrix(float **m, int n){
    for(int i=0; i<n; i++){
        for(int j=0;j<n;j++)
            printf("%f ", m[i][j]);
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
    start();
    dotProduct(c, a, b, N);
    printf("%llu\n", stop());

    for(int i=0; i<NUM_EVENTS; i++){
        printf("%lld\n",values[i]);
    }
}
