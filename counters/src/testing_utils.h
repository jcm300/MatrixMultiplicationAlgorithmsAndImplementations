#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include <papi.h>

#define N 500
#define TIME_RESOLUTION 1000000
#define NUM_EVENTS 2

void start (void);
long long unsigned stop (void);
