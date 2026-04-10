#include <hip/hip_runtime.h>
#include <stdio.h>

int main() {
    greeting_kernel<<<8, 64>>>();

    hipDeviceSynchronize();

    return 0;
}