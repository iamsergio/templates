
#include <QtWidgets>

#include <stdlib.h>
#include <unistd.h>
#include <EGL/egl.h>
#include <GL/gl.h>

static EGLint const attribute_list[] = {
    EGL_RED_SIZE, 1,
    EGL_GREEN_SIZE, 1,
    EGL_BLUE_SIZE, 1,
    EGL_ALPHA_SIZE, 1,
    EGL_DEPTH_SIZE, 1,
    EGL_STENCIL_SIZE, 0,
    EGL_NONE
};

EGLint contextAttributes[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
};

int render_to(QWindow *window)
{
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
    EGLint num_config;

    /* get an EGL display connection */
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (display == EGL_NO_DISPLAY) {
        qWarning() << "Failed to get display";
        return -1;
    }

    /* initialize the EGL display connection */
    if (!eglInitialize(display, NULL, NULL)) {
        qWarning() << "Failed to initialize!";
        return -1;
    }

    /* get an appropriate EGL frame buffer configuration */
    auto res = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
    if (!res) {
        qWarning() << "Failed to pick config!";
        return -1;
    }


    // Some info
    qDebug() << "Version:" << eglQueryString(display, EGL_VERSION)
             << "; Vendor:" << eglQueryString(display, EGL_VENDOR)
             << "; APIS:" << eglQueryString(display, EGL_CLIENT_APIS);


    /* create an EGL rendering context */
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributes);
    if (context == EGL_NO_CONTEXT) {
        qWarning() << "Failed to create context";
        return -1;
    }

    /* create an EGL window surface */
    surface = eglCreateWindowSurface(display, config, window->winId(), NULL);

    /* connect the context to the surface */
    eglMakeCurrent(display, surface, surface, context);

    /* clear the color buffer */
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    eglSwapBuffers(display, surface);
    return 0;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWindow window;
    window.resize(200, 200);
    window.show();

    render_to(&window);

    return app.exec();
}