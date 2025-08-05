/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "window.h"
#include "scene.h"

#include <QPainter>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QWheelEvent>
#include <QKeyEvent>

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent)
{
    setSurfaceType(QWindow::OpenGLSurface);
    auto f = format();
    f.setSwapInterval(1);
    f.setSamples(16);
    f.setAlphaBufferSize(8);
    f.setDepthBufferSize(24);
#ifdef Q_OS_MACOS
    f.setMajorVersion(3);
    f.setMinorVersion(2);
    f.setProfile(QSurfaceFormat::CoreProfile);
#endif
    setFormat(f);
}

OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::renderLater()
{
    requestUpdate();
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        render();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed())
        render();
}

void OpenGLWindow::resizeEvent(QResizeEvent *)
{
    if (!m_context)
        return;
    m_context->makeCurrent(this);
    updateViewportSize();
}

void OpenGLWindow::wheelEvent(QWheelEvent *ev)
{
    QMatrix4x4 m = m_scene->viewMatrix();

    if (ev->angleDelta().y() > 0) {
        m.scale(0.8);
    } else if (ev->angleDelta().y() < 0) {
        m.scale(1 / 0.8);
    }

    m_scene->setViewMatrix(m);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *ev)
{
    QMatrix4x4 m = m_scene->viewMatrix();
    if (ev->key() == Qt::Key_Left) {
        m.rotate(5 + (ev->isAutoRepeat() ? 15 : 0), 0, 0, 1);
        m_scene->setViewMatrix(m);
    } else if (ev->key() == Qt::Key_Right) {
        m.rotate(-5 - (ev->isAutoRepeat() ? 15 : 0), 0, 0, 1);
        m_scene->setViewMatrix(m);
    } else if (ev->key() == Qt::Key_Down) {
        m.rotate(-5 - (ev->isAutoRepeat() ? 15 : 0), 1, 0, 0);
        m_scene->setViewMatrix(m);
    } else if (ev->key() == Qt::Key_Up) {
        m.rotate(5 + (ev->isAutoRepeat() ? 15 : 0), 1, 0, 0);
        m_scene->setViewMatrix(m);
    }
}

void OpenGLWindow::updateViewportSize()
{
    const qreal retinaScale = devicePixelRatio();
    int w = width() * retinaScale;
    int h = height() * retinaScale;
    glViewport(0, 0, w, h);
}


void OpenGLWindow::render()
{
    if (!isExposed() || !m_scene)
        return;

    bool needsInitialize = false;
    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();
        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        updateViewportSize();
        m_time.start();
        auto glver = (const char*)::glGetString(GL_VERSION);
        qDebug() << "Using gl version" << glver;
    }

    m_scene->render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();

    ++m_frames;

   if (m_frames % 100 == 0) {
        qDebug() << "Frames: " << m_frames / (m_time.elapsed() / 1000.0);
    }
}

void OpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

void OpenGLWindow::setScene(Scene *scene)
{
    if (scene != m_scene) {
        m_scene = scene;
        if (scene)
            connect(scene, &SceneItem::needsRendering, this, &OpenGLWindow::renderLater, Qt::UniqueConnection);
        render();
    }
}
