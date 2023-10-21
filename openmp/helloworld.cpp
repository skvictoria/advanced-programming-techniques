#include <stdio.h>
#include <omp.h>

int main()
{
#pragma omp parallel // creates thread
    {
        printf("Hello world!");
    }
}