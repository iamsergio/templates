/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "pieceitem.h"
#include <QImage>

PieceItem::PieceItem(Piece piece, OpenGLWindow *w, SceneItem *parent)
    : SceneItem(w, parent)
    , m_piece(piece)
{
    Q_ASSERT(!piece.isEmpty());
    m_objName = "Piece";
    QMatrix4x4 modelToWorld;
    modelToWorld.scale(1 / 8.0);
    setModelToWordMatrix(modelToWorld);

    setShaderPaths(QStringLiteral(":/shaders/piece.vert"), QStringLiteral(":/shaders/piece.frag"));
}

void PieceItem::initializeImpl()
{
    const QString filename = ":/assets/2d/" + m_piece.filename();
    QImage image(filename);

    if (image.isNull()) {
        qWarning() << "PieceItem::initializeImpl() null image" << filename;
        Q_ASSERT(false);
        return;
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    const GLfloat vertices[] = {
        /*vertices*/ -1, -1,  /*texture coords*/0, 1, // bottom-left
                     -1, 1,                     0, 0, // top-left
                      1, 1,                     1, 0,
                      1, -1,                    1, 1
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(m_posAttr);
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    m_texAttr = m_program->attributeLocation("texcoord");
    glEnableVertexAttribArray(m_texAttr);
    glVertexAttribPointer(m_texAttr, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    GLuint ebo;
    glGenBuffers(1, &ebo);
    GLuint elements[] = {
        0, 1, 2,
        0, 2, 3
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image = image.convertToFormat(QImage::Format_ARGB32);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.constBits());
    glGenerateMipmap(GL_TEXTURE_2D);
}

void PieceItem::setChessPosition(QPoint chessPos)
{
    const QPointF p = chessPositionToBoardSpace(chessPos);
    m_positionInBoard.translate(p.x() , p.y(), 0);
    setModelToWordMatrix(m_positionInBoard * m_modelToWorldMatrix);
}

QPointF PieceItem::chessPositionToBoardSpace(QPoint chessPos) const
{
    const qreal squareLength = (1 - -1) / 8.0;

    return { -1 + chessPos.x() * squareLength + (squareLength / 2),
             -1 + chessPos.y() * squareLength + (squareLength / 2) };
}

void PieceItem::renderImpl()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, m_tex);
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
