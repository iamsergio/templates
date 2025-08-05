/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef WINDOW_H
#define WINDOW_H

#include <QWindow>
#include <QTime>
#include <QOpenGLFunctions>

class QOpenGLContext;
class QOpenGLPaintDevice;
class Scene;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = nullptr);
    ~OpenGLWindow();

    void setAnimating(bool animating);
    void setScene(Scene *scene);
    void render();
    void renderLater();

protected:
    bool event(QEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;
    void resizeEvent(QResizeEvent *) override;
    void wheelEvent(QWheelEvent *) override;
    void keyPressEvent(QKeyEvent *ev) override;


private:
    void updateViewportSize();
    void renderScene();
    bool m_animating = false;
    bool m_initialized = false;
    QOpenGLContext * m_context = nullptr;
    Scene *m_scene = nullptr;
    int m_frames = 0;
    QTime m_time;
};

#endif
