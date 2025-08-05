/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "boarditem.h"

#include <QFile>

static const QColor s_whiteSquareColor = Qt::white;

BoardItem::BoardItem(OpenGLWindow *w, SceneItem *parent)
    : SceneItem(w, parent)
{
    m_objName = "Board";
    setShaderPaths(QStringLiteral(":/shaders/board.vert"), QStringLiteral(":/shaders/board.frag"));

    // Board coincides with the world, so use the identity matrix
    m_modelToWorldMatrix = QMatrix4x4();

    setupDefaultBoard();
}

void BoardItem::setPieces(const QVector<Piece> &pieces)
{
    if (pieces.size() != 64) {
        qWarning() << "BoardItem::setPieces: Expected 64 pieces";
        Q_ASSERT(false);
        return;
    }

    m_pieces = pieces;
    createPieceItems();
}

void BoardItem::initializeImpl()
{
    glGenVertexArrays(1, &m_boardRectVAO);
    glBindVertexArray(m_boardRectVAO);

    GLuint boardRectVBO = 0;
    glGenBuffers(1, &boardRectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boardRectVBO);

    QVector<float> vertices;

    // Top
    vertices << -1 << -1 << 0 /*normal=*/ << 0 << 0 << 1;
    vertices <<  1 << -1 << 0 /*normal=*/ << 0 << 0 << 1;
    vertices <<  1 <<  1 << 0 /*normal=*/ << 0 << 0 << 1;
    vertices <<  1 <<  1 << 0 /*normal=*/ << 0 << 0 << 1;
    vertices << -1 << -1 << 0 /*normal=*/ << 0 << 0 << 1;
    vertices << -1 <<  1 << 0 /*normal=*/ << 0 << 0 << 1;


    int stride = 6 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.constData(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(m_posAttr);

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, stride, 0);
    Q_ASSERT(!glGetError());

    const int normalAttr = m_program->attributeLocation("aNormal");

    glEnableVertexAttribArray(normalAttr);
    glVertexAttribPointer(normalAttr, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));

    GLuint boardRectVBO2 = 0;
    glGenBuffers(1, &boardRectVBO2);
    glBindBuffer(GL_ARRAY_BUFFER, boardRectVBO2);
    const int colorTypeAttr = m_program->attributeLocation("colorType");
    Q_ASSERT(colorTypeAttr != -1);
    const QVector<float> colors(vertices.size() / 3, 5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), colors.constData(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorTypeAttr);
    glVertexAttribPointer(colorTypeAttr, 1, GL_FLOAT, GL_FALSE, 0, 0);

    glGenVertexArrays(1, &m_boardBlackSquaresVAO);
    glBindVertexArray(m_boardBlackSquaresVAO);

    vertices.clear();
    const qreal square_length = (1 - -1) / 8.0;

    for (int row = 0; row < 8; ++row) {
        for (int col = (row % 2 == 0) ? 0 : 1 ;col < 8; col += 2) {
            vertices << -1 + square_length * col << -1 + square_length * row << 0 << 1 /*normal=*/ << 0 << 0 << 1;
            vertices << -1 + square_length * col << -1 + square_length + square_length * row << 0 << 1 /*normal=*/ << 0 << 0 << 1;
            vertices << -1 + square_length + square_length * col << -1 + square_length + square_length * row << 0 << 1 /*normal=*/ << 0 << 0 << 1;

            vertices << -1 + square_length * col << -1 + square_length * row << 0 << 1 /*normal=*/ << 0 << 0 << 1;
            vertices << -1 + square_length + square_length * col << -1 + square_length * row << 0 << 1 /*normal=*/ << 0 << 0 << 1;
            vertices << -1 + square_length + square_length * col << -1 + square_length + square_length * row << 0 << 1 /*normal=*/ << 0 << 0 << 1;
        }
    }

    stride = 7 * sizeof(float);
    GLuint boardBlackSquaresVBO = 0;
    glGenBuffers(1, &boardBlackSquaresVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boardBlackSquaresVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(m_posAttr);

    glVertexAttribPointer(colorTypeAttr, 1, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(colorTypeAttr);

    glEnableVertexAttribArray(normalAttr);
    glVertexAttribPointer(normalAttr, 3, GL_FLOAT, GL_FALSE, stride, (void*) (4 * sizeof(float)));

    glGenVertexArrays(1, &m_boardSidesVAO);
    glBindVertexArray(m_boardSidesVAO);

    const QString filename = ":/assets/wood.jpg";
    QImage image(filename);

    if (image.isNull()) {
        qWarning() << "PieceItem::initializeImpl() null image" << filename;
        Q_ASSERT(false);
        return;
    }

    vertices.clear();
    // board sides
    vertices << -1 << -1 << 0 /*normal=*/    << 0 << -1 << 0;
    vertices << 1 << -1  << 0 /*normal=*/    << 0 << -1 << 0;
    vertices << -1 << -1 << -0.1 /*normal=*/ << 0 << -1 << 0;
    vertices << -1 << -1 << -0.1 /*normal=*/ << 0 << -1 << 0;
    vertices << 1 << -1  << 0 /*normal=*/    << 0 << -1 << 0;
    vertices << 1 << -1  << -0.1 /*normal=*/ << 0 << -1 << 0;

    vertices << -1 << 1  << 0 /*normal=*/   << 0 << 1 << 0;
    vertices << 1 << 1  << 0 /*normal=*/    << 0 << 1 << 0;
    vertices << -1 << 1 << -0.1 /*normal=*/ << 0 << 1 << 0;
    vertices << -1 << 1 << -0.1 /*normal=*/ << 0 << 1 << 0;
    vertices << 1 << 1  << 0 /*normal=*/    << 0 << 1 << 0;
    vertices << 1 << 1  << -0.1 /*normal=*/ << 0 << 1 << 0;

    vertices << -1 << -1 << 0 /*normal=*/    << -1 << 0 << 0;
    vertices << -1 << 1 << 0 /*normal=*/     << -1 << 0 << 0;
    vertices << -1 << 1 << -0.1 /*normal=*/  << -1 << 0 << 0;
    vertices << -1 << 1 << -0.1 /*normal=*/  << -1 << 0 << 0;
    vertices << -1 << -1 << 0 /*normal=*/    << -1 << 0 << 0;
    vertices << -1 << -1 << -0.1 /*normal=*/ << -1 << 0 << 0;

    vertices << 1 << -1 << 0 /*normal=*/    << 1 << 0 << 0;
    vertices << 1 << 1 << 0 /*normal=*/     << 1 << 0 << 0;
    vertices << 1 << 1 << -0.1 /*normal=*/  << 1 << 0 << 0;
    vertices << 1 << 1 << -0.1 /*normal=*/  << 1 << 0 << 0;
    vertices << 1 << -1 << 0 /*normal=*/    << 1 << 0 << 0;
    vertices << 1 << -1 << -0.1 /*normal=*/ << 1 << 0 << 0;

    // Bottom
    vertices << -1 << -1 << -0.1 /*normal=*/ << 0 << 0 << -1;
    vertices << 1 << -1 << -0.1 /*normal=*/  << 0 << 0 << -1;
    vertices << 1 << 1 << -0.1 /*normal=*/   << 0 << 0 << -1;
    vertices << 1 << 1 << -0.1 /*normal=*/   << 0 << 0 << -1;
    vertices << -1 << -1 << -0.1 /*normal=*/ << 0 << 0 << -1;
    vertices << -1 << 1 << -0.1 /*normal=*/  << 0 << 0 << -1;

    glGenTextures(1, &m_tex);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image = image.convertToFormat(QImage::Format_ARGB32);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.constBits());
    glGenerateMipmap(GL_TEXTURE_2D);

    stride = 6 * sizeof(float);
    GLuint boardSidesVBO = 0;
    glGenBuffers(1, &boardSidesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boardSidesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(m_posAttr);

    glEnableVertexAttribArray(normalAttr);
    glVertexAttribPointer(normalAttr, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
}

void BoardItem::createPieceItems()
{
    qDeleteAll(m_pieceItems);

    for (int i = 0; i < 64; ++i) {
        const Piece &piece = m_pieces[i];
        if (m_pieces[i].isEmpty())
            continue;

        auto pieceItem = new PieceItem(piece, m_window, this);
        pieceItem->setChessPosition(sequentialIndexToChessPos(i));
        m_pieceItems << pieceItem;
    }
}

void BoardItem::setupDefaultBoard()
{
    QVector<Piece> pieces;
    pieces.reserve(64);

    pieces << Piece(Type_Rook, Color_White);
    pieces << Piece(Type_Knight, Color_White);
    pieces << Piece(Type_Bishop, Color_White);
    pieces << Piece(Type_King, Color_White);
    pieces << Piece(Type_Queen, Color_White);
    pieces << Piece(Type_Bishop, Color_White);
    pieces << Piece(Type_Knight, Color_White);
    pieces << Piece(Type_Rook, Color_White);

    for (int i = 0; i < 8; ++i)
        pieces << Piece(Type_Pawn, Color_White);

    for (int i = 0; i < 32; ++i)
        pieces << Piece(Type_None, Color_None);

    for (int i = 0; i < 8; ++i)
        pieces << Piece(Type_Pawn, Color_Black);

    pieces << Piece(Type_Rook, Color_Black);
    pieces << Piece(Type_Knight, Color_Black);
    pieces << Piece(Type_Bishop, Color_Black);
    pieces << Piece(Type_King, Color_Black);
    pieces << Piece(Type_Queen, Color_Black);
    pieces << Piece(Type_Bishop, Color_Black);
    pieces << Piece(Type_Knight, Color_Black);
    pieces << Piece(Type_Rook, Color_Black);

    setPieces(pieces);
}

QPoint BoardItem::sequentialIndexToChessPos(int index) const
{
    const int row = index / 8;
    const int col = index % 8;

    return QPoint(col, row);
}

void BoardItem::renderImpl()
{
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(m_boardRectVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // Draw the white base

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(m_boardBlackSquaresVAO); // Draw the black squares, with same Z as the base
    glDrawArrays(GL_TRIANGLES, 0, 192);

    for (auto p : m_pieceItems) {
        p->render(); // Draw the 2D piecs with same Z as the base
    }

    m_program->bind();
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glEnable(GL_DEPTH_TEST); // Re-enable depth testing for the 3d parts (board sides)
    glBindVertexArray(m_boardSidesVAO);
    glDrawArrays(GL_TRIANGLES, 0, (6 * 4) + 6);
}
