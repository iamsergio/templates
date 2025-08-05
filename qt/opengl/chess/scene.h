/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SCENE_H
#define SCENE_H

#include "boarditem.h"
#include "pieceitem.h"

#include <QObject>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector3D>

class Scene : public SceneItem
{
    Q_OBJECT
public:
    explicit Scene(OpenGLWindow *window);
    void renderImpl() override;
    void initializeImpl() override;

    Light light() const;
    void setLight(Light);

signals:
    void lightChanged(Light);

private:
    QVector3D m_lightPosition;
    BoardItem *const m_boardItem;
    PieceItem::List m_pieces;
    Light m_light;
};

#endif
