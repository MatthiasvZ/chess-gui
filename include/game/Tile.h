#ifndef TILE_H
#define TILE_H

#include <vector>

enum Piece : int
{
    none,
    pawn_w, pawn_b,
    knight_w, knight_b,
    bishop_w, bishop_b,
    rook_w, rook_b,
    queen_w, queen_b,
    king_w, king_b
};

class Tile
{
    public:
        Tile(unsigned char posX, unsigned char posY);
        virtual ~Tile();

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
