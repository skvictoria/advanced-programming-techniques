/*
Author: Seulgi Kim
Class: ECE6122 A
Last Date Modified: 11/07/2023

Description:

This code gets number of walkers and number of each steps that walkers take,
simulate a random walk (either east, west, south, north) of each walker, and calculate the average distance.

*/
#include <cuda.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>
#include <iostream>
#include <numeric>

unsigned long long num_steps = 10000;
unsigned long long num_walkers = 1000;

/**
 * @brief This method extract the valid natural number from the input stream from the user.
 * @param strInputStream The incoming string stream
 * @param validNum return extracted valid natural number.
 * @return 1 if succeed extracting, 0 if not.
*/
uint8_t ExtractValidNaturalNum(const char* strInputStream, unsigned long long &validNum)
{
    uint8_t u8Success = 1U;
    char cSubString;
    std::string strresult(strInputStream);

    if (strresult.length() > 0U)
    {   
        u8Success = 1U;
        // for each letter in a number,
        for (uint8_t u8LetterIdx = 0U; u8LetterIdx < strresult.length(); u8LetterIdx++)
        {
            cSubString = (strresult.substr(u8LetterIdx, 1))[0];
            // if any one of the input is not digit, failed.
            if (isdigit(cSubString) == 0)
            {
                u8Success = 0U;
                break;
            }
        }

        if (u8Success == 1U)
        {
            validNum = strtoull(strInputStream, nullptr, 10);
        }
    }
    else
    {
        u8Success = 0U;
    }

    return u8Success;
}

/**
 * @brief This method calculate the distance for walkers.
 * @param state random state.
 * @param distance distance for walker.
 * @param numWalkers number of walkers from the user.
 * @param numSteps number of steps from the user.
 * @param seed seed for generating random state.
*/
__global__ void calculateDistanceperWalker(curandState* state, float* distance, unsigned long long numWalkers
                                        , unsigned long long numSteps, int seed) 
{
    unsigned long long tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= numWalkers)
    {
        return;
    }
    float randVal;
    unsigned long long x = 0, y = 0;
    curand_init(static_cast<unsigned int>(seed), tid, 0, &state[tid]);

    // #1 Optimization: reduce if-condition for random walk (either east, west, south, north)
        // east  if 0.0f  < randVal < 0.25f
        // west  if 0.25f < randVal < 0.5f
        // south if 0.5f  < randVal < 0.75f
        // north if 0.75f < randVal < 1.0f
    for (unsigned long long step = 0; step < numSteps; ++step)
    {
        randVal = curand_uniform(&state[tid]);
        x += (randVal < 0.25f) - (randVal >= 0.25f && randVal < 0.5f);
        y += (randVal >= 0.75f) - (randVal >= 0.5f && randVal < 0.75f);
    }
    distance[tid] = sqrtf(x * x + y * y);
}

/**
 * @brief This method calculate the distance for walkers.
 * @param walkers walkers array for host memory
 * @param d_walkers walkers array for device memory
 * @param d_states states array (for generating random states) for device memory
*/
void NormalCudaMalloc(float* walkers, float* d_walkers, curandState* d_states)
{
    // Initialize the value.
    float averageDistance = 0.0f;
    float microsec;
    cudaEvent_t startEvent, stopEvent; 
    cudaStream_t stream;
    cudaStreamCreate(&stream);
    cudaEventCreate(&startEvent);
    cudaEventCreate(&stopEvent);

    // Start Timing.
    cudaEventRecord(startEvent, 0);

    // Allocate host memory.
    walkers = (float*)malloc(sizeof(float) * num_walkers);

    // Allocate device memory.
    cudaMalloc((void**)&d_walkers, sizeof(float) * num_walkers);
    cudaMalloc((void**)&d_states, sizeof(curandState) * num_walkers);

    calculateDistanceperWalker<<<(num_walkers+256)/256, 256, 0, stream>>>(d_states, d_walkers, num_walkers, num_steps, time(NULL));

    // Transfer data back to host memory.
    // #2 Optimization: Use Cuda Async operation - overlap of kernel execution & Data Transfer.
    cudaMemcpyAsync(walkers, d_walkers, sizeof(float) * num_walkers, cudaMemcpyDeviceToHost, stream);

    // Make sure block CPU thread before finializing the cuda async processes.
    cudaStreamSynchronize(stream);
    averageDistance = std::accumulate(walkers, walkers + num_walkers, 0.0f) / num_walkers;

    // Deallocate device memory.
    cudaFree(d_walkers);
    cudaFree(d_states);

    // Deallocate host memory.
    free(walkers);

    // Destroy Stream.
    cudaStreamDestroy(stream);

    // Finish Timing.
    cudaEventRecord(stopEvent, 0);
    cudaEventSynchronize(stopEvent);
    cudaEventElapsedTime(&microsec, startEvent, stopEvent);
    std::cout << "Normal CUDA memory Allocation:" << std::endl;
    std::cout << "    Time to calculate(microsec): " << microsec * 1000 << std::endl;
    std::cout << "    Average distance from origin: " << averageDistance << std::endl;
}

/**
 * @brief This method calculate the distance for walkers.
 * @param walkers walkers array for host memory
 * @param d_walkers walkers array for device memory
 * @param d_states states array (for generating random states) for device memory
*/
void PinnedCudaMalloc(float** walkers, float* d_walkers, curandState* d_states)
{
    // Initialize the value.
    float averageDistance = 0.0f;
    float microsec;
    cudaEvent_t startEvent, stopEvent;
    cudaStream_t stream;
    cudaStreamCreate(&stream);
    cudaEventCreate(&startEvent);
    cudaEventCreate(&stopEvent);

    // Start Timing.
    cudaEventRecord(startEvent, 0);

    // Allocate host memory.
    cudaMallocHost((void**)walkers, sizeof(float) * num_walkers);

    // Allocate device memory.
    cudaMalloc((void**)&d_walkers, sizeof(float) * num_walkers);
    cudaMalloc((void**)&d_states, sizeof(curandState) * num_walkers);

    calculateDistanceperWalker<<<(num_walkers+256)/256, 256, 0, stream>>>(d_states, d_walkers, num_walkers, num_steps, time(NULL));

    // Transfer data back to host memory.
    // #3 Optimization: Use Cuda Async operation - overlap of kernel execution & Data Transfer.
    cudaMemcpyAsync(*walkers, d_walkers, sizeof(float) * num_walkers, cudaMemcpyDeviceToHost, stream);
    
    // Make sure block CPU thread before finializing the cuda async processes.
    cudaStreamSynchronize(stream);
    averageDistance = std::accumulate(*walkers, *walkers + num_walkers, 0.0f) / num_walkers;

    // Deallocate device memory.
    cudaFree(d_walkers);
    cudaFree(d_states);

    // Deallocate host memory.
    cudaFreeHost(*walkers);

    // Destroy Stream.
    cudaStreamDestroy(stream);

    // Finish Timing.
    cudaEventRecord(stopEvent, 0);
    cudaEventSynchronize(stopEvent);
    cudaEventElapsedTime(&microsec, startEvent, stopEvent);
    std::cout << "Pinned CUDA memory Allocation:" << std::endl;
    std::cout << "    Time to calculate(microsec): " << microsec * 1000 << std::endl;
    std::cout << "    Average distance from origin: " << averageDistance << std::endl;
}

/**
 * @brief This method calculate the distance for walkers.
 * @param d_walkers walkers array for device memory.
 * @param d_states states array (for generating random states) for device memory
*/
void ManagedCudaMalloc(float* d_walkers, curandState* d_states)
{
    // Initialize the value.
    float averageDistance = 0.0f;
    float microsec;
    cudaEvent_t startEvent, stopEvent; 
    cudaEventCreate(&startEvent);
    cudaEventCreate(&stopEvent);

    // Start Timing.
    cudaEventRecord(startEvent, 0);

    // Allocate memory.
    cudaMallocManaged(&d_walkers, sizeof(float) * num_walkers);
    cudaMallocManaged(&d_states, sizeof(curandState) * num_walkers);

    calculateDistanceperWalker<<<(num_walkers+256)/256, 256>>>(d_states, d_walkers, num_walkers, num_steps, time(NULL));
    cudaDeviceSynchronize();
    averageDistance = std::accumulate(d_walkers, d_walkers + num_walkers, 0.0f) / num_walkers;

    // Deallocate device memory.
    cudaFree(d_walkers);
    cudaFree(d_states);

    // Finish Timing.
    cudaEventRecord(stopEvent, 0);
    cudaEventSynchronize(stopEvent);
    cudaEventElapsedTime(&microsec, startEvent, stopEvent);
    std::cout << "Managed CUDA memory Allocation:" << std::endl;
    std::cout << "    Time to calculate(microsec): " << microsec * 1000 << std::endl;
    std::cout << "    Average distance from origin: " << averageDistance << std::endl;
}

int main(int argc, char* argv[])
{
    // Initialize the variables.
    float *walkers = 0;
    float *d_walkers = 0;
    curandState *d_states = 0;

    // Get the input from user.
    for (int i = 1; i < argc - 1; ++i)
    {
        if (strcmp(argv[i], "-W") == 0)
        {
            if (ExtractValidNaturalNum(argv[++i], num_walkers) == 0U)
            {
                std::cout << "Invalid num_walkers" << std::endl;
                return EXIT_FAILURE;
            }
        }
        if (strcmp(argv[i], "-I") == 0)
        {
            if (ExtractValidNaturalNum(argv[++i], num_steps) == 0U)
            {
                std::cout << "Invalid num_steps" << std::endl;
                return EXIT_FAILURE;
            }
        }
    }
    NormalCudaMalloc(walkers, d_walkers, d_states);
    
    PinnedCudaMalloc(&walkers, d_walkers, d_states);
    
    ManagedCudaMalloc(d_walkers, d_states);

    std::cout << "Bye" << std::endl;

    return EXIT_SUCCESS;
}