#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <X11/Xlib.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

void die(const std::string &msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    Display *x_display = XOpenDisplay(nullptr);
    if (!x_display) die("Could not open X display");

    int screen = DefaultScreen(x_display);
    Window root = RootWindow(x_display, screen);

    int width = 800, height = 600;
    Window win = XCreateSimpleWindow(x_display, root, 0, 0, width, height, 1,
                                     BlackPixel(x_display, screen), WhitePixel(x_display, screen));
    XMapWindow(x_display, win);
    XFlush(x_display);

    EGLDisplay egl_display = eglGetDisplay((EGLNativeDisplayType)x_display);
    if (egl_display == EGL_NO_DISPLAY) die("Failed to get EGL display");
    if (!eglInitialize(egl_display, nullptr, nullptr)) die("Failed to initialize EGL");

    static const EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8,
        EGL_NONE
    };

    EGLint num_configs;
    EGLConfig config;
    if (!eglChooseConfig(egl_display, attribs, &config, 1, &num_configs) || num_configs < 1)
        die("Failed to choose EGL config");

    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLContext egl_context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT, context_attribs);
    if (egl_context == EGL_NO_CONTEXT) die("Failed to create EGL context");

    EGLSurface egl_surface = eglCreateWindowSurface(egl_display, config, (EGLNativeWindowType)win, nullptr);
    if (egl_surface == EGL_NO_SURFACE) die("Failed to create EGL window surface");

    if (!eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context))
        die("Failed to make EGL context current");

    glClearColor(0.2f, 0.6f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(egl_display, egl_surface);

    std::cout << "EGL rendering complete. Press Enter to exit..." << std::endl;
    std::cin.get();

    eglDestroySurface(egl_display, egl_surface);
    eglDestroyContext(egl_display, egl_context);
    eglTerminate(egl_display);
    XDestroyWindow(x_display, win);
    XCloseDisplay(x_display);

    return 0;
}
