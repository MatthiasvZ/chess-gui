#ifndef PIECE_H
#define PIECE_H

#define WHITE 0
#define BLACK 1

static constexpr signed char NONE     = 0b0000'0000;
static constexpr signed char PAWN_W   = 0b0000'0001;
static constexpr signed char PAWN_B   = 0b1000'0001;
static constexpr signed char KNIGHT_W = 0b0000'0010;
static constexpr signed char KNIGHT_B = 0b1000'0010;
static constexpr signed char BISHOP_W = 0b0000'0100;
static constexpr signed char BISHOP_B = 0b1000'0100;
static constexpr signed char ROOK_W   = 0b0000'1000;
static constexpr signed char ROOK_B   = 0b1000'1000;
static constexpr signed char QUEEN_W  = 0b0001'0000;
static constexpr signed char QUEEN_B  = 0b1001'0000;
static constexpr signed char KING_W   = 0b0010'0000;
static constexpr signed char KING_B   = 0b1010'0000;

class Piece
{
    public:
        Piece(signed char piece);

        inline bool color()    const { return data & 0b1000'0000; }
        inline bool isEmpty()  const { return !data; }
        inline bool isPawn()   const { return data & 0b0000'0001; }
        inline bool isKnight() const { return data & 0b0000'0010; }
        inline bool isBishop() const { return data & 0b0000'0100; }
        inline bool isRook()   const { return data & 0b0000'1000; }
        inline bool isQueen()  const { return data & 0b0001'0000; }
        inline bool isKing()   const { return data & 0b0010'0000; }

        signed char data;
        inline bool operator==(Piece b)       const { return this->data == b.data; }
        inline bool operator==(signed char b) const { return this->data == b; }
};

#endif // PIECE_H
