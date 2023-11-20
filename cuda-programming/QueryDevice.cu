#include <stdio.h> 
#include <iostream>
// http://developer.download.nvidia.com/compute/cuda/4_0/toolkit/docs/online/group__CUDART__DEVICE_g5aa4f47938af8276f08074d09b7d520c.html
using namespace std;

int main() {
  int nDevices;

  cudaGetDeviceCount(&nDevices);
  for (int i = 0; i < nDevices; i++) {
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, i);
    printf("Device Number: %d\n", i);
    printf("  Device name: %s\n", prop.name);
    printf("  Integrated Device: %s\n", prop.integrated?"True":"False");

    printf("  Memory Clock Rate (KHz): %d\n",
           prop.memoryClockRate);
    printf("  Memory Bus Width (bits): %d\n",
           prop.memoryBusWidth);
    printf("  Peak Memory Bandwidth (GB/s): %f\n\n",
           2.0*prop.memoryClockRate*(prop.memoryBusWidth/8)/1.0e6);

	cout << "  Num. Multiprocessors: " << prop.multiProcessorCount << endl;
	
	cout << "  Max. Threads per block: " << prop.maxThreadsPerBlock << endl;
	// Maximum size of each dimension of a grid.
	cout << "  Max Grid Size: (" 
	     << prop.maxGridSize[0] << ", "
		 << prop.maxGridSize[1] << ", "
		 << prop.maxGridSize[2] << ")" << endl;
	cout << "  Max Block Size: (" 
	     << prop.maxThreadsDim[0] << ", "
		 << prop.maxThreadsDim[1] << ", "
		 << prop.maxThreadsDim[2] << ")" << endl;
		 
	cout << endl;
		 
  }
}