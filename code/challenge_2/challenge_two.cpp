#include <hip/hip_runtime.h>
#include <stdio.h>

__global__ void addTwoArrays1024(float array1[], float array2[]) {
    // Finding the index using blocks and thread so that index is independent of block and threads assigned to kernel
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    array1[idx] = array1[idx] + array2[idx];
}

int main() {
    int N = 1024;

    float* h_array_1 = (float*)malloc(N * sizeof(float));
    float* h_array_2 = (float*)malloc(N * sizeof(float));
    float *d_array_1, *d_array_2;

    // Random init
    for (int i = 0; i < N; i++) {
        h_array_1[i] = (float)i + 1.0f;
        h_array_2[i] = (float)i + 1.0f;
    }

    // Allocating memory on gpu
    hipMalloc(&d_array_1, N * sizeof(float));
    hipMalloc(&d_array_2, N * sizeof(float));

    // Copying values from cpu ram to gpu ram
    hipMemcpy(d_array_1, h_array_1, N * sizeof(float), hipMemcpyHostToDevice);
    hipMemcpy(d_array_2, h_array_2, N * sizeof(float), hipMemcpyHostToDevice);

    // Calling the kernel (can use any combination of block size and thread dunno what is best) - async call
    addTwoArrays1024<<<8, 128>>>(d_array_1, d_array_2);

    // This implicitly waits for the async kernel call to finish
    hipMemcpy(h_array_1, d_array_1, N * sizeof(float), hipMemcpyDeviceToHost);

    // This is not explicitly needed but good practice as we can catch errors here
    hipError_t err = hipDeviceSynchronize();

    // This is error handling practice for gpu kernel calls
    if (err != hipSuccess) {
        printf("GPU Error: %s\n", hipGetErrorString(err));
    } else {
        for (int i = 0; i < N; i++) {
            printf("%f -- %f\n", h_array_1[i], h_array_2[i]);
        }  
    }

    free(h_array_1);
    free(h_array_2);
    hipFree(d_array_1);
    hipFree(d_array_2);

    return 0;
}