/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "sceneitem.h"

#include "scene.h"

#include <QFile>
#include <QDebug>

SceneItem::SceneItem(OpenGLWindow *window, SceneItem *parent)
    : QObject(parent)
    , m_window(window)
    , m_parentItem(parent)
{

    SceneItem *p = this;
    do {
        if (p) {
            if ((m_scene = qobject_cast<Scene*>(p)))
                break;
            p = p->parentItem();
        }
    } while (p);

    m_material.shininess = 16;
    m_material.ambient = { 1.0, 1.0, 1.0 };
    m_material.diffuse = { 1.0, 1.0, 1.0 };
    m_material.specular = { 1.0, 1.0, 1.0 };
}

void SceneItem::render()
{
    if (!QOpenGLContext::currentContext())
        return;

    if (!m_initialized) {
        initialize();
        m_initialized = true;
    }

    if (m_program) {
        m_program->bind();
        updateUniforms();
    }

    renderImpl();

    if (m_program)
        m_program->release();
}

void SceneItem::initialize()
{
    initializeOpenGLFunctions();

    //qDebug() << "SceneItem::initialize() name=" << m_objName;
    if (!m_vertexShaderPath.isEmpty() && !m_fragShaderPath.isEmpty()) {
        const QByteArray vertexContents = readShader(m_vertexShaderPath);
        const QByteArray fragContents = readShader(m_fragShaderPath);
        m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexContents.constData());
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragContents.constData());
        const bool success = m_program->link();
        Q_ASSERT(success);
        m_posAttr = m_program->attributeLocation("position");
        Q_ASSERT(m_posAttr != -1);
    }

    initializeImpl();
}

SceneItem *SceneItem::parentItem() const
{
    return m_parentItem;
}

QMatrix4x4 SceneItem::finalViewMatrix() const
{
    return (m_parentItem ? m_parentItem->finalViewMatrix() : QMatrix4x4()) * m_viewMatrix;
}

QMatrix4x4 SceneItem::viewMatrix() const
{
    return m_viewMatrix;
}

void SceneItem::setViewMatrix(const QMatrix4x4 &m)
{
    if (m_viewMatrix != m) {
        m_viewMatrix = m;
        emit needsRendering();
    }
}

void SceneItem::updateUniforms()
{
    m_program->setUniformValue("projectionAndViewMatrix", finalViewMatrix());
    m_program->setUniformValue("modelToWorldMatrix", m_modelToWorldMatrix);
    m_program->setUniformValue("viewPos", QVector3D(0, 0, 0.5));

    m_program->setUniformValue("material.ambient", m_material.ambient);
    m_program->setUniformValue("material.diffuse", m_material.diffuse);
    m_program->setUniformValue("material.specular", m_material.specular);
    m_program->setUniformValue("material.shininess", m_material.shininess);

    Light light = scene()->light();
    m_program->setUniformValue("light.ambient", light.ambient);
    m_program->setUniformValue("light.position", light.position);
    m_program->setUniformValue("light.diffuse", light.diffuse);
    m_program->setUniformValue("light.specular", light.specular);
}

void SceneItem::setModelToWordMatrix(const QMatrix4x4 &m)
{
    if (m_modelToWorldMatrix != m) {
        m_modelToWorldMatrix = m;
        emit needsRendering();
    }
}

void SceneItem::setShaderPaths(const QString &vertexPath, const QString &fragPath)
{
    m_vertexShaderPath = vertexPath;
    m_fragShaderPath = fragPath;

    if (!fragPath.isEmpty() && !QFile::exists(fragPath))
        qFatal("Could not find frag shader");

    if (!vertexPath.isEmpty() && !QFile::exists(vertexPath))
        qFatal("Could not find vertex shader");
}

QByteArray SceneItem::readShader(const QString &filename) const
{
    QFile shader(filename);
    if (!shader.open(QFile::ReadOnly)) {
        qWarning() << "Failed to read shader" << shader.fileName();
        return {};
    }

    return shader.readAll();
}

QOpenGLContext *SceneItem::context() const
{
    return QOpenGLContext::currentContext();
}

const Scene *SceneItem::scene() const
{
    const Scene *scene = m_scene;

    if (!scene)
        scene = qobject_cast<const Scene*>(this);

    if (!scene) {
        qFatal("No scene for item");
    }

    return scene;
}

void SceneItem::checkError()
{

}
