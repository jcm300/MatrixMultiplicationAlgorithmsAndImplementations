#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include <papi.h>

#define TIME_RESOLUTION 1000000
#define N 50
#define NUM_EVENTS 3

void start (void);
long long unsigned stop (void);
