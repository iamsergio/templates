#include <cstdlib>
#include <cstring>
#include <cuda_runtime.h>
#include <npp.h>

#include <iostream>
#include <nppi_filtering_functions.h>


void *generate_source_image(int width, int height, int channels, int depth)
{
    const auto size = width * height * channels * depth;
    auto data = malloc(size);

    memset(data, 0, size);

    std::cout << "malloc: " << size / 1024.0 / 1024.0 << " MB\n";
    return data;
}

void *allocateDeviceMemory(int width, int height, int channels, int depth)
{
    void *d_data = nullptr;
    cudaMalloc(&d_data, width * height * channels * depth);
    return d_data;
}

#define WIDTH 3840
#define HEIGHT 2160
#define CHANNELS 4
#define DEPTH 1 // byte

int main(int argc, char **argv)
{
    const int size = WIDTH * HEIGHT * CHANNELS * DEPTH;
    auto src = generate_source_image(WIDTH, HEIGHT, CHANNELS, DEPTH);
    auto d_src = static_cast<Npp8u *>(allocateDeviceMemory(WIDTH, HEIGHT, CHANNELS, DEPTH));
    auto d_dst = static_cast<Npp8u *>(allocateDeviceMemory(WIDTH, HEIGHT, CHANNELS, DEPTH));

    // to device:
    cudaMemcpy(d_src, src, size, cudaMemcpyHostToDevice);

    NppiSize oMaskSize = { 5, 5 };
    NppiPoint oAnchor = { oMaskSize.width / 2, oMaskSize.height / 2 };
    NppiPoint oSrcOffset = { 0, 0 };
    NppiSize oSrcSize = { WIDTH, HEIGHT };
    NppiSize oSizeROI = { WIDTH, HEIGHT };
    const Npp32s pitchBytes = WIDTH * CHANNELS * DEPTH;
    std::cout << "aligned ? " << pitchBytes % 64 << std::endl;

    nppiFilterBoxBorder_8u_C3R(
        d_src, pitchBytes, oSrcSize, oSrcOffset,
        d_dst, pitchBytes, oSizeROI, oMaskSize, oAnchor,
        NPP_BORDER_REPLICATE);

    // to host:
    cudaMemcpy(src, d_dst, size, cudaMemcpyDeviceToHost);

    free(src);
    cudaFree(d_src);
    return 0;
}
