#include <fcntl.h>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <unistd.h>
#include <gbm.h>

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

void die(const std::string &msg) {
  perror(msg.c_str());
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <drm_card_path>" << std::endl;
    std::cerr << "Example: " << argv[0] << " /dev/dri/card1"  << std::endl;
    return EXIT_FAILURE;
  }
  const char *drm_card_name = argv[1];
  int fd = open(drm_card_name, O_RDWR);
  if (fd < 0) die("Could not open DRM device");

  drmModeResPtr res = drmModeGetResources(fd);
  if (!res) {
    drmClose(fd);
    die("Could not get DRM resources.");
  }

  drmModeConnectorPtr connector = nullptr;
  drmModeEncoderPtr encoder = nullptr;
  drmModeModeInfoPtr mode = nullptr;
  for (int i = 0; i < res->count_connectors; ++i) {
    connector = drmModeGetConnector(fd, res->connectors[i]);
    if (connector && connector->connection == DRM_MODE_CONNECTED && connector->count_modes > 0) {
      mode = &connector->modes[0];
      for (int j = 0; j < res->count_encoders; ++j) {
        encoder = drmModeGetEncoder(fd, res->encoders[j]);
        if (encoder && encoder->encoder_id == connector->encoder_id) break;
        if (encoder) drmModeFreeEncoder(encoder);
        encoder = nullptr;
      }
      if (encoder) break;
    }
    if (connector) drmModeFreeConnector(connector);
    connector = nullptr;
  }
  if (!connector || !encoder || !mode) {
    drmModeFreeResources(res);
    drmClose(fd);
    die("No connected monitor found.");
  }

  drmModeCrtcPtr crtc = nullptr;
  for (int i = 0; i < res->count_crtcs; ++i) {
    crtc = drmModeGetCrtc(fd, res->crtcs[i]);
    if (crtc && crtc->crtc_id == encoder->crtc_id) break;
    if (crtc) drmModeFreeCrtc(crtc);
    crtc = nullptr;
  }
  if (!crtc) {
    drmModeFreeResources(res);
    drmModeFreeConnector(connector);
    drmModeFreeEncoder(encoder);
    drmClose(fd);
    die("No available CRTC found.");
  }

  struct gbm_device *gbm_dev = gbm_create_device(fd);
  if (!gbm_dev) die("Failed to create GBM device.");

  struct gbm_surface *gbm_surf = gbm_surface_create(
    gbm_dev, mode->hdisplay, mode->vdisplay,
    GBM_FORMAT_XRGB8888, GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING
  );
  if (!gbm_surf) die("Failed to create GBM surface.");

  EGLDisplay egl_display = EGL_NO_DISPLAY;

  PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
      (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
  if (eglGetPlatformDisplayEXT) {
      egl_display = eglGetPlatformDisplayEXT(EGL_PLATFORM_GBM_KHR, gbm_dev, NULL);
  }

  if (egl_display == EGL_NO_DISPLAY) die("Failed to get EGL display.");
  if (!eglInitialize(egl_display, nullptr, nullptr)) die("Failed to initialize EGL.");

  static const EGLint attribs[] = {
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8,
    EGL_NONE
  };

  EGLint num_configs;
  eglGetConfigs(egl_display, nullptr, 0, &num_configs);
  std::vector<EGLConfig> configs(num_configs);
  eglGetConfigs(egl_display, configs.data(), num_configs, &num_configs);

  EGLConfig chosen_config = nullptr;
  std::cout << "Found numConfigs=" << num_configs << "\n";
  for (int i = 0; i < num_configs; ++i) {
      EGLint id;
      eglGetConfigAttrib(egl_display, configs[i], EGL_NATIVE_VISUAL_ID, &id);
      if (id == GBM_FORMAT_XRGB8888) {
          chosen_config = configs[i];
          break;
      }
  }
  if (!chosen_config) die("No matching EGLConfig for GBM_FORMAT_XRGB8888");

  EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
  };

  EGLContext egl_context = eglCreateContext(egl_display, chosen_config, EGL_NO_CONTEXT, context_attribs);
  if (egl_context == EGL_NO_CONTEXT) die("Failed to create EGL context.");

  assert(eglGetError() == EGL_SUCCESS);
  EGLSurface egl_surface = eglCreateWindowSurface(egl_display, chosen_config, (EGLNativeWindowType)gbm_surf, nullptr);
  if (egl_surface == EGL_NO_SURFACE) {
      EGLint error = eglGetError();
          fprintf(stderr, "eglCreateWindowSurface failed: 0x%X\n", error);
      die("Failed to create EGL window surface.");
  }

  if (!eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context))
    die("Failed to make EGL context current.");

  glClearColor(0.2f, 0.6f, 0.8f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  eglSwapBuffers(egl_display, egl_surface);

  struct gbm_bo *bo = gbm_surface_lock_front_buffer(gbm_surf);
  uint32_t handle = gbm_bo_get_handle(bo).u32;
  uint32_t pitch = gbm_bo_get_stride(bo);
  uint32_t fb_id;
  if (drmModeAddFB(fd, mode->hdisplay, mode->vdisplay, 24, 32, pitch, handle, &fb_id) < 0)
    die("Could not create DRM framebuffer");

  if (drmModeSetCrtc(fd, crtc->crtc_id, fb_id, 0, 0, &connector->connector_id, 1, mode) < 0)
    die("Failed to set CRTC.");

  std::cout << "EGL rendering complete. Press Enter to exit..." << std::endl;
  std::cin.get();

  drmModeSetCrtc(fd, crtc->crtc_id, crtc->buffer_id, crtc->x, crtc->y,
                 &connector->connector_id, 1, &crtc->mode);

  gbm_surface_release_buffer(gbm_surf, bo);
  gbm_surface_destroy(gbm_surf);
  gbm_device_destroy(gbm_dev);
  drmModeFreeCrtc(crtc);
  drmModeFreeEncoder(encoder);
  drmModeFreeConnector(connector);
  drmModeFreeResources(res);
  drmClose(fd);

  return 0;
}
