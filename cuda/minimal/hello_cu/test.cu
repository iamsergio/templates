#include <stdio.h>

__global__ void cuda_hello()
{
    printf("Hello World from GPU!%d\n", threadIdx.x);
}

void my_kernel_wrapper()
{
    cuda_hello<<<1, 1>>>();
}
