#include <stdio.h>
#include <omp.h>

main(int argc, char *argv[])
{
    int nthreads, tid;
#pragma omp parallel private(tid)
    {
        // private variable: uninitialized.
        tid = omp_get_thread_num(); // intialize
        printf("Hello world from thread %d\n", tid);

        if(tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads %d\n", nthreads);
        }
    }// all threads join master thread and terminate.
}