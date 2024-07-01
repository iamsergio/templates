#include <cuda_runtime.h>

#include <iostream>

int main()
{

    void *dst = nullptr;
    void *src = nullptr;
    const int bytesPerPixel = 2;
    const int dstWidth = 50;
    const int dstHeight = 50;

    const int dstWidthBytes = dstWidth * bytesPerPixel;

    const int srcWidth = 1000;
    const int srcHeight = 1000;
    const int srcWidthBytes = srcWidth * bytesPerPixel;

    auto res = cudaMalloc(&dst, dstWidthBytes * dstHeight);
    if (res) {
        std::cout << "Could not cudaMalloc" << res << "\n";
        return res;
    }

    src = malloc(srcWidthBytes * srcHeight);
    if (!src) {
        std::cout << "Could not malloc"
                  << "\n";
        return res;
    }

    // important it's dst width in bytes but height in pixels
    res =
        cudaMemcpy2D(dst, dstWidthBytes, src, srcWidthBytes, dstWidthBytes, dstHeight,
                     cudaMemcpyHostToDevice);

    if (res) {
        std::cout << cudaGetErrorString(res) << "\n";
    }


    return 0;
}
