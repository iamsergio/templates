/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SCENEITEM_H
#define SCENEITEM_H

#include "window.h"
#include "light.h"

#include <QObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_2_Core>
#include <QOpenGLContext>
#include <QMatrix4x4>
#include <QObject>

class Scene;

class SceneItem : public QObject, protected QOpenGLFunctions_3_2_Core
{
    Q_OBJECT
public:
    explicit SceneItem(OpenGLWindow *window, SceneItem *parent = nullptr);
    void render();
    void initialize();
    SceneItem *parentItem() const;

    QMatrix4x4 finalViewMatrix() const;
    QMatrix4x4 viewMatrix() const;
    void setViewMatrix(const QMatrix4x4 &);

signals:
    void needsRendering();

protected:
    void updateUniforms();
    void setModelToWordMatrix(const QMatrix4x4 &);
    void setShaderPaths(const QString &vertexPath, const QString &fragPath);
    void checkError();
    QByteArray readShader(const QString &filename) const;
    virtual void initializeImpl() = 0;
    virtual void renderImpl() = 0;
    QOpenGLContext *context() const;
    bool m_initialized = false;
    OpenGLWindow *const m_window;
    QOpenGLShaderProgram *m_program = nullptr;
    int m_posAttr = -1;
    GLuint m_vao = 0;
    QString m_objName;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_modelToWorldMatrix;
private:
    const Scene *scene() const;

    QString m_vertexShaderPath;
    QString m_fragShaderPath;
    SceneItem *const m_parentItem;
    Scene *m_scene = nullptr;
    Material m_material;
};

#endif
