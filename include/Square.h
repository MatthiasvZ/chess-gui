#ifndef SQUARE_H
#define SQUARE_H

#include "Position.h"
#include "Piece.h"

class Square
{
    public:
        Square(Position pos, Piece piece)
            : pos(pos), piece(piece) {}

        Position pos;
        Piece piece;
};

#endif // SQUARE_H
