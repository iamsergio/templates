#include <cuda_runtime.h>

extern void my_kernel_wrapper();

int main()
{
    my_kernel_wrapper();
    cudaDeviceSynchronize();

    return 0;
}
