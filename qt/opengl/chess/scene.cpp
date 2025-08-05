/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "scene.h"

Scene::Scene(OpenGLWindow *window)
    : SceneItem(window, nullptr)
    , m_boardItem(new BoardItem(window, this))
{
    m_objName = "Scene";

    // This is actually the projection matrix
    m_viewMatrix.ortho(-1, 1, -1, 1, -1.0f, 1.0f);    

    m_light.position = { 0, 0, 0.3f };
    m_light.ambient = { 0.4f, 0.4f, 0.4f };
    m_light.diffuse = { 0.5f, 0.5f, 0.5f };
    m_light.specular = { 0.2f, 0.2f, 0.2f };
}

void Scene::initializeImpl()
{
}

void Scene::renderImpl()
{
    glClearDepth(1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_boardItem->render();
}

Light Scene::light() const
{
    return m_light;
}

void Scene::setLight(Light l)
{
    if (m_light != l) {
        m_light = l;
        emit lightChanged(l);
        m_window->renderLater();
    }
}
