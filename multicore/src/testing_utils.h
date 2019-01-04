#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define N 512
#define BLOCK_SIZE 4

#define TIME_RESOLUTION 1000000

void start (void);
long long unsigned stop (void);
