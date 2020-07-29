#ifndef TILE_H
#define TILE_H

#include "game/Common.h"
#include <vector>

class Tile
{
    public:
        Tile(unsigned char posX, unsigned char posY);

        inline std::vector<float> getVertices() const { return vertices; }
        inline Piece getPiece() const { return piece; }
        inline void setPiece(Piece piece)
        {
            this->piece = piece;
        }
    protected:

    private:
        std::vector<float> vertices;
        Piece piece;
};

#endif // TILE_H
