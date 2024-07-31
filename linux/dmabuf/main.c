#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Function to allocate a DMA-BUF using dma-heap
int allocate_dmabuf(size_t size)
{
    int fd, dmabuf_fd;
    struct dma_heap_allocation_data alloc_data;

    fd = open("/dev/dma_heap/system", O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    memset(&alloc_data, 0, sizeof(alloc_data));
    alloc_data.len = size;
    alloc_data.fd_flags = O_RDWR;

    if (ioctl(fd, DMA_HEAP_IOCTL_ALLOC, &alloc_data) < 0) {
        perror("ioctl");
        close(fd);
        return -1;
    }

    dmabuf_fd = alloc_data.fd;
    close(fd);

    return dmabuf_fd;
}

// Function to simulate using the DMA-BUF in another device
void use_dmabuf(int dmabuf_fd) {

}

int main() {
    size_t size = 1024 * 1024; // 1MB buffer
    int dmabuf_fd = allocate_dmabuf(size);
    if (dmabuf_fd < 0) {
        fprintf(stderr, "Failed to allocate DMA-BUF\n");
        return EXIT_FAILURE;
    }

    // use_dmabuf(dmabuf_fd);
    //
    // close(dmabuf_fd);
    return EXIT_SUCCESS;
}
