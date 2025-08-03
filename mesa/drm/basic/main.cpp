#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>

// DRM headers
#include <gbm.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

// A simple structure to hold our framebuffer and related info
struct FrameBuffer {
  uint32_t fb_id;
  struct gbm_bo *bo;
  void *map_data;
  uint32_t pitch;
  int fd;
};

// Function to handle errors and print them
void die(const std::string &msg) {
  perror(msg.c_str());
  exit(EXIT_FAILURE);
}

// Function to get a DRM property from a connector or CRTC
uint32_t get_drm_property_id(int fd, const std::string &name,
                             drmModeObjectPropertiesPtr props) {
  for (int i = 0; i < props->count_props; ++i) {
    drmModePropertyPtr prop = drmModeGetProperty(fd, props->props[i]);
    if (prop && name == prop->name) {
      drmModeFreeProperty(prop);
      return props->props[i];
    }
    if (prop)
      drmModeFreeProperty(prop);
  }
  return 0; // Not found
}

int main(int argc, char *argv[]) {
  // Step 1: Open the DRM device file
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <drm_card_path>" << std::endl;
    std::cerr << "Example: " << argv[0] << " card0" << std::endl;
    return EXIT_FAILURE;
  }

  const char *drm_card_name = argv[1];

  // Step 1: Open the DRM device file
  int fd = open(drm_card_name, O_RDWR);
  // int fd = drmOpen(drm_card_name, NULL); not working
  if (fd < 0) {
    std::string error_msg = "Could not open DRM device (";
    error_msg += drm_card_name;
    error_msg += "). Are you running as root or in the video group?";
    die(error_msg);
  }
  std::cout << "DRM device opened successfully: " << drm_card_name << std::endl;

  // Step 2: Get basic DRM resources
  drmModeResPtr res = drmModeGetResources(fd);
  if (!res) {
    drmClose(fd);
    die("Could not get DRM resources.");
  }

  // Step 3: Find a suitable connector, encoder, and mode
  drmModeConnectorPtr connector = nullptr;
  drmModeEncoderPtr encoder = nullptr;
  drmModeModeInfoPtr mode = nullptr;

  std::cout << "got num connectors=" << res->count_connectors << "\n";
  for (int i = 0; i < res->count_connectors; ++i) {
    connector = drmModeGetConnector(fd, res->connectors[i]);
    std::cout << "Testing connector: " << i << "\n";
    if (connector && connector->connection == DRM_MODE_CONNECTED &&
        connector->count_modes > 0) {
      std::cout << "Found a connected connector: " << res->connectors[i]
                << std::endl;
      mode = &connector->modes[0]; // Use the first mode
      std::cout << "Using mode: " << mode->name << " (" << mode->hdisplay << "x"
                << mode->vdisplay << ")" << std::endl;

      // Find a suitable encoder
      std::cout << "got num encoders=" << res->count_encoders << "\n";
      for (int j = 0; j < res->count_encoders; ++j) {
        encoder = drmModeGetEncoder(fd, res->encoders[j]);
        if (encoder) {
            if(encoder->encoder_id == connector->encoder_id) {
                std::cout << "Found a matching encoder: " << res->encoders[j]
                    << std::endl;
                break;
            }
            drmModeFreeEncoder(encoder);
            encoder = nullptr;
        }
      }
      if (encoder)
        break;
    }
    drmModeFreeConnector(connector);
    connector = nullptr;
  }

  if (!connector || !encoder || !mode) {
    std::cerr << "No connected monitor found. Exiting." << std::endl;
    drmModeFreeResources(res);
    drmClose(fd);
    return 1;
  }

  std::string connector_name = drmModeGetConnectorTypeName(connector->connector_type);
  std::cout << "Connector type name: " << connector_name << "\n";


  // Step 4: Find an available CRTC (Cathode Ray Tube Controller)
  drmModeCrtcPtr crtc = nullptr;
  std::cout << "got num crtcs=" << res->count_crtcs << "\n";
  for (int i = 0; i < res->count_crtcs; ++i) {
    crtc = drmModeGetCrtc(fd, res->crtcs[i]);
    if (crtc && crtc->crtc_id == encoder->crtc_id) {
      std::cout << "Found a matching CRTC: " << res->crtcs[i] << std::endl;
      break;
    }
    drmModeFreeCrtc(crtc);
    crtc = nullptr;
  }
  if (!crtc) {
    // Fallback: find any free CRTC
    for (int i = 0; i < res->count_crtcs; ++i) {
      crtc = drmModeGetCrtc(fd, res->crtcs[i]);
      if (crtc && crtc->buffer_id == 0) {
        std::cout << "Found a free CRTC: " << res->crtcs[i] << std::endl;
        break;
      }
      drmModeFreeCrtc(crtc);
      crtc = nullptr;
    }
  }
  if (!crtc) {
    std::cerr << "No available CRTC found. Exiting." << std::endl;
    drmModeFreeResources(res);
    drmModeFreeConnector(connector);
    drmModeFreeEncoder(encoder);
    drmClose(fd);
    return 1;
  }

  // Step 5: Initialize GBM
  struct gbm_device *gbm_dev = gbm_create_device(fd);
  if (!gbm_dev) {
    std::cerr << "Failed to create GBM device." << std::endl;
    drmModeFreeResources(res);
    drmModeFreeConnector(connector);
    drmModeFreeEncoder(encoder);
    drmModeFreeCrtc(crtc);
    drmClose(fd);
    return 1;
  }

  std::cout << "Created GMB device" << "\n";

  // only for EGL:
  // Step 6: Create a GBM surface for the framebuffer
  // struct gbm_surface *gbm_surf =
  //     gbm_surface_create(gbm_dev, mode->hdisplay, mode->vdisplay,
  //                        GBM_FORMAT_XRGB8888, // Use a 32-bit format
  //                        GBM_BO_USE_SCANOUT | GBM_BO_USE_WRITE);
  // if (!gbm_surf) {
  //   std::cerr << "Failed to create GBM surface." << std::endl;
  //   gbm_device_destroy(gbm_dev);
  //   drmModeFreeResources(res);
  //   drmModeFreeConnector(connector);
  //   drmModeFreeEncoder(encoder);
  //   drmModeFreeCrtc(crtc);
  //   drmClose(fd);
  //   return 1;
  // }


  struct gbm_bo *bo = gbm_bo_create(gbm_dev, mode->hdisplay, mode->vdisplay,
                                    GBM_FORMAT_XRGB8888,
                                    GBM_BO_USE_SCANOUT | GBM_BO_USE_WRITE);
  std::cout << "Created GMB bo" << "\n";

  // only for EGL:
  // Step 7: Create an initial GBM buffer object
  // struct gbm_bo *bo = gbm_surface_lock_front_buffer(gbm_surf);
  // if (!bo) {
  //   std::cerr << "Failed to lock GBM front buffer." << std::endl;
  //   gbm_surface_destroy(gbm_surf);
  //   gbm_device_destroy(gbm_dev);
  //   drmModeFreeResources(res);
  //   drmModeFreeConnector(connector);
  //   drmModeFreeEncoder(encoder);
  //   drmModeFreeCrtc(crtc);
  //   drmClose(fd);
  //   return 1;
  // }

  std::cout << "Locked front buffer" << "\n";

  // Step 8: Create a DRM framebuffer from the GBM buffer object
  uint32_t width = gbm_bo_get_width(bo);
  uint32_t height = gbm_bo_get_height(bo);
  uint32_t pitch = gbm_bo_get_stride(bo);
  uint32_t handle = gbm_bo_get_handle(bo).u32;
  uint32_t fb_id;

  if (drmModeAddFB(fd, width, height, 24, 32, pitch, handle, &fb_id) < 0) {
    die("Could not create DRM framebuffer");
  }

  // Step 9: Map the buffer into memory
  void *map_data;
  void *vmap_addr = gbm_bo_map(bo, 0, 0, width, height, GBM_BO_TRANSFER_WRITE,
                               &pitch, &map_data);
  if (!vmap_addr) {
    die("Failed to map GBM buffer object");
  }

  // Step 10: Draw a pattern to the buffer (this is where you would render your
  // int[][] array) The buffer is in XRGB8888 format, which means 4 bytes per
  // pixel: [0] = unused, [1]=Red, [2]=Green, [3]=Blue
  uint32_t *pixel_data = static_cast<uint32_t *>(vmap_addr);
  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      // Simple color gradient pattern
      uint8_t r = (x * 255) / width;
      uint8_t g = (y * 255) / height;
      uint8_t b = 0;
      pixel_data[y * (pitch / 4) + x] = (r << 16) | (g << 8) | b;
    }
  }

  // Step 11: Set the CRTC to use our new framebuffer. This is what displays the
  // image.
  if (drmModeSetCrtc(fd, crtc->crtc_id, fb_id, 0, 0, &connector->connector_id,
                     1, mode) < 0) {
    die("Failed to set CRTC.");
  }
  std::cout << "DRM buffer set and displayed on screen." << std::endl;

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  // Step 12: Cleanup
  std::cout << "Cleaning up..." << std::endl;

  // Unset the CRTC
  drmModeSetCrtc(fd, crtc->crtc_id, crtc->buffer_id, crtc->x, crtc->y,
                 &connector->connector_id, 1, &crtc->mode);

  // Unmap and destroy buffer
  gbm_bo_unmap(bo, map_data);
  // gbm_surface_release_buffer(gbm_surf, bo);
  // gbm_surface_destroy(gbm_surf);
  gbm_device_destroy(gbm_dev);

  // Free resources and close fd
  drmModeFreeCrtc(crtc);
  drmModeFreeEncoder(encoder);
  drmModeFreeConnector(connector);
  drmModeFreeResources(res);
  drmClose(fd);

  std::cout << "Exiting program." << std::endl;

  return 0;
}
