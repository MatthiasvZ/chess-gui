#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "Square.h"

constexpr int ONGOING       =  2;
constexpr int WIN_FOR_WHITE =  1;
constexpr int DRAWN         =  0;
constexpr int WIN_FOR_BLACK = -1;

enum File : unsigned char
{
    a, b, c, d, e, f, g, h
};

class Rights
{
    public:
        Rights()
            : data(0) {}

        // Castling rights
        inline void set_k(bool n) { data ^= (-n ^ data) & 0b0000'0001; }
        inline void set_q(bool n) { data ^= (-n ^ data) & 0b0000'0010; }
        inline void set_K(bool n) { data ^= (-n ^ data) & 0b0000'0100; }
        inline void set_Q(bool n) { data ^= (-n ^ data) & 0b0000'1000; }

        inline bool k() const { return (data >> 0) & 0b0000'0001; }
        inline bool q() const { return (data >> 1) & 0b0000'0001; }
        inline bool K() const { return (data >> 2) & 0b0000'0001; }
        inline bool Q() const { return (data >> 3) & 0b0000'0001; }

        // En-passant rights
        inline void set_enPassant(File file) { data |= (file << 4); }
        inline void clear_enPassant() { data &= 0b0000'1111; }
        inline File enPassant() const { return static_cast<File>(data >> 4); }

    private:
        unsigned char data;
};

class Board
{
    public:
        Board(std::string fen);
        inline Piece& at(Position pos) { return squares[pos.d()].piece; }
        inline Piece  at(Position pos) const { return squares[pos.d()].piece; }

        bool toMove;
        Rights rights;
        unsigned char halfmoveClock;
        unsigned char fullmoveClock;

        int outcome;

    protected:

    private:
        std::vector<Square> squares;
};

#endif // BOARD_H
