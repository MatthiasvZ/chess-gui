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
    none, a, b, c, d, e, f, g, h
};

class Rights
{
    public:
        Rights()
            : _k(false), _q(false), _K(false), _Q(false), enPassantFile(File::none) {}

        // Castling rights
        inline void set_k(bool n) { _k = n; }
        inline void set_q(bool n) { _q = n; }
        inline void set_K(bool n) { _K = n; }
        inline void set_Q(bool n) { _Q = n; }

        inline bool k() const { return _k; }
        inline bool q() const { return _q; }
        inline bool K() const { return _K; }
        inline bool Q() const { return _Q; }

        // En-passant rights
        inline void set_enPassant(File file) { enPassantFile = file; }
        inline void clear_enPassant() { enPassantFile = File::none; }
        inline File enPassant() const { return enPassantFile; }

    private:
        bool _k;
        bool _q;
        bool _K;
        bool _Q;
        File enPassantFile;
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
