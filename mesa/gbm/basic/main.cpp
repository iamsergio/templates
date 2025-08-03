#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <gbm.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <xf86drm.h> // For DRM ioctl

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <DRI_NODE_PATH>" << std::endl;
    std::cerr << "Example: " << argv[0] << " /dev/dri/renderD128" << std::endl;
    return 1;
  }
  const char *dri_node_path = argv[1];

  // 1. Open the DRI device
  int drm_fd = open(dri_node_path, O_RDWR);
  if (drm_fd < 0) {
    std::cerr << "Failed to open DRM device " << dri_node_path << ": " << strerror(errno) << std::endl;
    return 1;
  }
  std::cout << "DRM device open\n";

  // 2. Create a GBM device from the file descriptor
  struct gbm_device *gbm_dev = gbm_create_device(drm_fd);
  if (!gbm_dev) {
    std::cerr << "Failed to create GBM device" << std::endl;
    close(drm_fd);
    return 1;
  }

  std::cout << "DRM device created: " <<   gbm_device_get_backend_name(gbm_dev) << "\n";

  // 3. Define buffer properties
  const int width = 640;
  const int height = 480;
  const uint32_t format = GBM_FORMAT_XRGB8888;
  const uint32_t flags = GBM_BO_USE_RENDERING;

  // 4. Allocate a GBM buffer object
  struct gbm_bo *bo = gbm_bo_create(gbm_dev, width, height, format, flags);
  if (!bo) {
    std::cerr << "Failed to create GBM buffer object" << std::endl;
    gbm_device_destroy(gbm_dev);
    close(drm_fd);
    return 1;
  }

  // would create dma buf
  // int dmabuf_fd = gbm_bo_get_fd(bo);

  std::cout << "Successfully created a GBM buffer object of size " << width
            << "x" << height << " pixels." << std::endl;

  // 5. Clean up
  gbm_bo_destroy(bo);
  gbm_device_destroy(gbm_dev);
  close(drm_fd);

  return 0;
}
