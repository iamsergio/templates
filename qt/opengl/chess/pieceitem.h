/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef PIECEITEM_H
#define PIECEITEM_H

#include "sceneitem.h"
#include "piece.h"

#include <QVector>

class PieceItem : public SceneItem
{
    Q_OBJECT
public:
    typedef QVector<PieceItem*> List;
    explicit PieceItem(Piece piece, OpenGLWindow *w, SceneItem *parent);
    void renderImpl() override;
    void initializeImpl() override;
    void setChessPosition(QPoint chessPos);
private:
    QPointF chessPositionToBoardSpace(QPoint chessPos) const;
    const Piece m_piece;
    GLuint m_tex;
    QMatrix4x4 m_positionInBoard;
    int m_texAttr = -1;
};

#endif
