//
//
// helloflops3offl
//
// A simple example that gets lots of Flops (Floating Point Operations) on
// Intel(r) Xeon Phi(tm) co-processors using offload plus  openmp to scale
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/time.h>
#include <cstdlib>
#include <thread>

using namespace std;

// dtime
//
// returns the current wall clock time
//
double dtime() {
    double tseconds = 0.0;
    struct timeval mytime;
    gettimeofday(&mytime,(struct timezone*)0);
    tseconds = (double)(mytime.tv_sec + mytime.tv_usec*1.0e-6);
    return( tseconds );
}

int getCores() {

    int cores = thread::hardware_concurrency();

    if (cores < 60)
        cores = cores/2;
    else
        cores = cores/4;

    return cores;
}

#define FLOPS_ARRAY_SIZE (1024*1024)
#define MAXFLOPS_ITERS 300

// number of float pt ops per calculation
#define FLOPSPERCALC 3
// define some arrays -
// make sure they are 64 byte aligned
// for best cache access


float __attribute__((aligned(32))) *fa;
float __attribute__((aligned(32))) *fb;


//
// Main program - pedal to the metal...calculate using tons o'flops!
//
int main(int argc, char *argv[] ) {
    int i,j,k;
    int numthreads, cores;
    double tstart, tstop, ttime;
    double gflops = 0.0;
    float a=1.1;

    //
    // initialize the compute arrays
    //
    //


    #pragma omp parallel
    {
        #pragma omp single
        {
        numthreads = omp_get_num_threads();
        }
    }


    printf("Initializing\r\n");

    //fa = (float*) malloc(FLOPS_ARRAY_SIZE*numthreads*sizeof(float)); 
    //fb = (float*) malloc(FLOPS_ARRAY_SIZE*numthreads*sizeof(float)); 

    // aligned memory allocation
    fa = (float*) _mm_malloc(FLOPS_ARRAY_SIZE*numthreads*sizeof(float), 32); 
    fb = (float*) _mm_malloc(FLOPS_ARRAY_SIZE*numthreads*sizeof(float), 32); 

    for(i=0; i<FLOPS_ARRAY_SIZE*numthreads; i++) {
        fa[i] = (float)i + 0.1;
        fb[i] = (float)i + 0.2;
    }
    // printf("Starting Compute on %d threads\r\n",numthreads);

    tstart = dtime();

    // scale the calculation across threads requested
    // need to set environment variables OMP_NUM_THREADS and KMP_AFFINITY
    #pragma omp for private(j,k)
    for (i=0; i<numthreads; i++) {
        // each thread will work it's own array section
        // calc offset into the right section
        int offset = i*(FLOPS_ARRAY_SIZE);

        // loop many times to get lots of calculations
        // this loop is only for benchmarking purposes
        for(j=0; j<MAXFLOPS_ITERS; j++) {
            // scale 1st array and add in the 2nd array
        //    #pragma vector aligned
            for(k=0; k<(FLOPS_ARRAY_SIZE); k++) {
                fa[k+offset] = a * fa[k+offset] + fb[k+offset];
            }
        }
    }
    tstop = dtime();
    // # of gigaflops we just calculated
    gflops = (double)( 1.0e-9*numthreads*FLOPS_ARRAY_SIZE*
                        MAXFLOPS_ITERS*FLOPSPERCALC);

    //elasped time
    ttime = tstop - tstart;
    //
    // Print the results
    //
    if ((ttime) > 0.0) {
        printf("Using %d threads\n", numthreads);
        printf("GFlops = %10.3lf, Secs = %10.3lf, GFlops per sec = %10.3lf\r\n",                   gflops, ttime, gflops/ttime);
    }
    return( 0 );
}
