#include "Board.h"

#include <iostream>
#include <cassert>

Board::Board(std::string fen)
    : outcome(ONGOING)
{
    squares.reserve(64);
    for (int ix {0}; ix < 8; ++ix)
        for (int iy {0}; iy < 8; ++iy)
            squares.push_back(Square(Position(ix, iy), Piece(NONE)));


    // Evaluate the FEN-string
    // https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

    unsigned long i {0};

    // Evaluate the piece positions
    for (int y {7}; y >= 0;)
    {
        for (int x {0}; x < 8; ++x)
        {
            switch (fen[i])
            {
                case ' ':
                    ++i;
                    goto toMove;
                    break;

                case '/':
                    --y;
                    --x;
                    break;
                case '1':
                    break;
                case '2':
                    x += 1;
                    break;
                case '3':
                    x += 2;
                    break;
                case '4':
                    x += 3;
                    break;
                case '5':
                    x += 4;
                    break;
                case '6':
                    x += 5;
                    break;
                case '7':
                    x += 6;
                    break;
                case '8':
                    x += 7;
                    break;

                case 'p':
                    squares[8*x + y].piece = Piece(PAWN_B);
                    break;
                case 'P':
                    squares[8*x + y].piece = Piece(PAWN_W);
                    break;

                case 'n':
                    squares[8*x + y].piece = Piece(KNIGHT_B);
                    break;
                case 'N':
                    squares[8*x + y].piece = Piece(KNIGHT_W);
                    break;

                case 'b':
                    squares[8*x + y].piece = Piece(BISHOP_B);
                    break;
                case 'B':
                    squares[8*x + y].piece = Piece(BISHOP_W);
                    break;

                case 'r':
                    squares[8*x + y].piece = Piece(ROOK_B);
                    break;
                case 'R':
                    squares[8*x + y].piece = Piece(ROOK_W);
                    break;

                case 'q':
                    squares[8*x + y].piece = Piece(QUEEN_B);
                    break;
                case 'Q':
                    squares[8*x + y].piece = Piece(QUEEN_W);
                    break;

                case 'k':
                    squares[8*x + y].piece = Piece(KING_B);
                    break;
                case 'K':
                    squares[8*x + y].piece = Piece(KING_W);
                    break;

                default:
                    std::cerr << "Invalid FEN! Unknown character '" << fen[i] << "' at pos " << i << std::endl;
                    assert(false);
            }
            ++i;
        }
    }

    // This character shows the player who's turn it is
    toMove:
    if (fen[i] == 'w')
        toMove = WHITE;
    else if (fen[i] == 'b')
        toMove = BLACK;
    else
        std::cerr << "Invalid FEN! Invalid character '" << fen[i] << "' at pos " << i << "; should be 'w' or 'b'.\n";
    i += 2;

    // Next are the castling rights
    for (; fen[i] != ' '; ++i)
    {
        switch (fen[i])
        {
            case 'k':
                rights.set_k(true);
                break;

            case 'q':
                rights.set_q(true);
                break;

            case 'K':
                rights.set_K(true);
                break;

            case 'Q':
                rights.set_Q(true);
                break;

            default:
                std::cerr << "Invalid FEN! Invalid character '" << fen[i] << "' at pos " << i << "; should be 'k', 'q', 'K' or 'Q'.\n";
        }
    }
    ++i;

    // Now the en-passant rights
    switch (fen[i])
    {
        case '-':
            i += 2;
            break;

        case 'a':
            rights.set_enPassant(File::a);
            i += 3;
            break;

        case 'b':
            rights.set_enPassant(File::b);
            i += 3;
            break;

        case 'c':
            rights.set_enPassant(File::c);
            i += 3;
            break;

        case 'd':
            rights.set_enPassant(File::d);
            i += 3;
            break;

        case 'e':
            rights.set_enPassant(File::e);
            i += 3;
            break;

        case 'f':
            rights.set_enPassant(File::f);
            i += 3;
            break;

        case 'g':
            rights.set_enPassant(File::g);
            i += 3;
            break;

        case 'h':
            rights.set_enPassant(File::h);
            i += 3;
            break;


        default:
            std::cerr << "Invalid FEN! Invalid character '" << fen[i] << "' at pos " << i << "; should be either '-', or a letter between 'a' and 'h' + a number.\n";
    }

    // Finally the move clocks
    std::string hmClock = "";
    for (; fen[i] >= '0' && fen[i] <= '9'; ++i)
        hmClock += fen[i];
    halfmoveClock = stoi(hmClock);
    ++i;
    std::string fmClock = "";
    for (; fen[i] >= '0' && fen[i] <= '9'; ++i)
        fmClock += fen[i];
    fullmoveClock = stoi(fmClock);
}
