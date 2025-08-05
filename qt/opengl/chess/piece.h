/*
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef PIECE_H
#define PIECE_H

#include <QString>

enum PieceType {
    Type_None = 0,
    Type_King,
    Type_Queen,
    Type_Knight,
    Type_Bishop,
    Type_Rook,
    Type_Pawn

};

enum PieceColor
{
    Color_None,
    Color_White,
    Color_Black
};

struct Piece
{
    Piece()
        : type(Type_None)
        , color(Color_None)
    {
    }

    explicit Piece(PieceType type, PieceColor color)
        : type(type)
        , color(color)
    {
    }

    bool isEmpty() const { return type == Type_None || color == Color_None; }

    QString typeName() const
    {
        switch (type)
        {
        case Type_King:
            return "rei";
        case Type_Queen:
            return "dama";
        case Type_Knight:
            return "cavalo";
        case Type_Bishop:
            return "bispo";
        case Type_Rook:
            return "torre";
        case Type_Pawn:
            return "peao";
        case Type_None:
            return "none";
        default:
            return "";
        }
    }

    QString colorName() const
    {
        if (color == Color_White)
            return "branco";
        else if (color == Color_Black)
            return "preto";
        else if (color == Color_None)
            return "none";
        else return "";
    }

    QString filename() const
    {
        return colorName() + "_" + typeName() + ".png";
    }

    PieceType type;
    PieceColor color;
};

#endif
