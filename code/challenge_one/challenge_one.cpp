#include <hip/hip_runtime.h>
#include <stdio.h>

// This __global__ specifies to the cpu (host) that this code is to be run inside gpu (device)
__global__ void greeting_kernel() {
    // Printing the ID using blockDim, blockId and threadId
    printf("Thread ID - %d\n", blockIdx.x * blockDim.x + threadIdx.x);
}

int main() {
    greeting_kernel<<<8, 64>>>();
    // same as using this macro to call gpu functions hipLaunchKernelGGL(greeting_kernel, dim3(8), dim3(64), 0, 0);

    /**
        hipLaunchKernelGGL(
        kernel_name,      // 1. The name of your __global__ function
        grid_dims,        // 2. How many Blocks (using dim3)
        block_dims,       // 3. How many Threads per block (using dim3)
        shared_mem,       // 4. Amount of dynamic shared memory (usually 0 for now)
        stream,           // 5. Which GPU stream to use (usually 0 for now)
        arg1, arg2...     // 6. The actual arguments for your function
    );
    */

    // Waits until gpu is done
    hipDeviceSynchronize();

    return 0;
}