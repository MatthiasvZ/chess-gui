#include <iostream> // TODO: remove
#include "Move.h"


bool moveIsLegal(Move move, const Board& board)
{
    const Piece pieceToMove   = board.at(move.pos1);
    const Piece pieceToLandOn = board.at(move.pos2);
    const auto x1 = move.pos1.x();
    const auto y1 = move.pos1.y();
    const auto x2 = move.pos2.x();
    const auto y2 = move.pos2.y();

    if (pieceToMove.isEmpty())
        return false;
    if (pieceToMove.color() != board.toMove)
        return false;
    if (pieceToMove.color() == pieceToLandOn.color() && !pieceToLandOn.isEmpty())
        return false;
    if (move.pos1 == move.pos2)
        return false;

    switch (pieceToMove.data)
    {
        case PAWN_W:
            if ((x2 == x1 + 1 || x2 == x1 - 1) && (y2 == y1 + 1 && pieceToLandOn != NONE))
                return true; // standard capturing
            if (y1 == 4 && y2 == 5 && (x2 == x1 - 1 || x2 == x1 + 1))
                if (board.rights.enPassant() == x2)
                    return true; // en-passant
            if (x2 != x1)
                return false; // otherwise disallow horizontal moves
            if (pieceToLandOn != NONE)
                return false; // or ones that capture
            if (y2 == y1 + 1)
                return true; // moving one square at a time
            if (y1 == 1 && y2 == 3 && board.at(Position(x1, 2)) == NONE)
                return true; // moving two squares from the starting position
            break;

        case PAWN_B:
            if ((x2 == x1 + 1 || x2 == x1 - 1) && (y2 == y1 - 1 && pieceToLandOn != NONE))
                return true; // standard capturing
            if (y1 == 3 && y2 == 2 && (x2 == x1 - 1 || x2 == x1 + 1))
                if (board.rights.enPassant() == x2)
                    return true; // en-passant
            if (x2 != x1)
                return false; // otherwise disallow horizontal moves
            if (pieceToLandOn != NONE)
                return false; // or ones that capture
            if (y2 == y1 - 1)
                return true; // moving one square at a time
            if (y1 == 6 && y2 == 4 && board.at(Position(x1, 5)) == NONE)
                return true; // moving two squares from the starting position
            break;

        case KNIGHT_W: [[fallthrough]];
        case KNIGHT_B:
            if (x2 == x1 + 2 && y2 == y1 + 1)
                return true;
            if (x2 == x1 + 2 && y2 == y1 - 1)
                return true;
            if (x2 == x1 - 2 && y2 == y1 + 1)
                return true;
            if (x2 == x1 - 2 && y2 == y1 - 1)
                return true;
            if (x2 == x1 + 1 && y2 == y1 + 2)
                return true;
            if (x2 == x1 - 1 && y2 == y1 + 2)
                return true;
            if (x2 == x1 + 1 && y2 == y1 - 2)
                return true;
            if (x2 == x1 - 1 && y2 == y1 - 2)
                return true;
            break;

        case BISHOP_W: [[fallthrough]];
        case BISHOP_B:
            if (x1 - y1 == x2 - y2) // '/'
            {
                for (int ix {x1 + 1}, iy {y1 + 1}; ix < x2; ix++, iy++)
                    if (board.at(Position(ix, iy)) != NONE)
                        return false;
                for (int ix {x1 - 1}, iy {y1 - 1}; ix > x2; ix--, iy--)
                    if (board.at(Position(ix, iy)) != NONE)
                        return false;
                return true;
            }
            if (x1 + y1 == x2 + y2) // '\'
            {
                for (int ix {x1 + 1}, iy {y1 - 1}; ix < x2; ix++, iy--)
                    if (board.at(Position(ix, iy)) != NONE)
                        return false;
                for (int ix {x1 - 1}, iy {y1 + 1}; ix > x2; ix--, iy++)
                    if (board.at(Position(ix, iy)) != NONE)
                        return false;
                return true;
            }
            break;

        case ROOK_W: [[fallthrough]];
        case ROOK_B:
            for (int ix {x1 + 1}; ix < x2; ix++)
                if (board.at(Position(ix, y1)) != NONE)
                    return false;
            for (int ix {x1 - 1}; ix > x2; ix--)
                if (board.at(Position(ix, y1)) != NONE)
                    return false;
            for (int iy {y1 + 1}; iy < y2; iy++)
                if (board.at(Position(x1, iy)) != NONE)
                    return false;
            for (int iy {y1 - 1}; iy > y2; iy--)
                if (board.at(Position(x1, iy)) != NONE)
                    return false;
            if (x1 == x2 || y1 == y2)
                return true;
            break;

        case QUEEN_W: [[fallthrough]];
        case QUEEN_B:
            if (x1 == x2 || y1 == y2)
            {
                for (int ix {x1 + 1}; ix < x2; ix++)
                    if (board.at(Position(ix, y1)) != NONE)
                        return false;
                for (int ix {x1 - 1}; ix > x2; ix--)
                    if (board.at(Position(ix, y1)) != NONE)
                        return false;
                for (int iy {y1 + 1}; iy < y2; iy++)
                    if (board.at(Position(x1, iy)) != NONE)
                        return false;
                for (int iy {y1 - 1}; iy > y2; iy--)
                    if (board.at(Position(x1, iy)) != NONE)
                        return false;
                return true;
            }
            if (x1 - y1 == x2 - y2)
            {
                for (int ix {x1 + 1}, iy {y1 + 1}; ix < x2; ix++, iy++)
                    if (board.at(Position(ix, iy)) != NONE)
                        return false;
                for (int ix {x1 - 1}, iy {y1 - 1}; ix > x2; ix--, iy--)
                    if (board.at(Position(ix, iy)) != NONE)
                        return false;
                return true;
            }
            if (x1 + y1 == x2 + y2)
            {
                for (int ix {x1 + 1}, iy {y1 - 1}; ix < x2; ix++, iy--)
                    if (board.at(Position(ix, iy)) != NONE)
                        return false;
                for (int ix {x1 - 1}, iy {y1 + 1}; ix > x2; ix--, iy++)
                    if (board.at(Position(ix, iy)) != NONE)
                        return false;
                return true;
            }
            break;

        case KING_W:
            if ((x2 == x1 + 1 && y2 == y1) ||
                (x2 == x1 + 1 && y2 == y1 + 1) ||
                (x2 == x1 + 1 && y2 == y1 - 1) ||
                (x2 == x1 - 1 && y2 == y1) ||
                (x2 == x1 - 1 && y2 == y1 + 1) ||
                (x2 == x1 - 1 && y2 == y1 - 1) ||
                (y2 == y1 + 1 && x2 == x1) ||
                (y2 == y1 - 1 && x2 == x1))
                return true;
            if (x2 == 2 && y2 == 0 && board.rights.Q() && board.at(Position(1, 0)) == NONE &&
                board.at(Position(2, 0)) == NONE && board.at(Position(3, 0)) == NONE)
                return true;
            if (x2 == 6 && y2 == 0 && board.rights.K() && board.at(Position(5, 0)) == NONE &&
                board.at(Position(6, 0)) == NONE)
                return true;
            break;

        case KING_B:
            if ((x2 == x1 + 1 && y2 == y1) ||
                (x2 == x1 + 1 && y2 == y1 + 1) ||
                (x2 == x1 + 1 && y2 == y1 - 1) ||
                (x2 == x1 - 1 && y2 == y1) ||
                (x2 == x1 - 1 && y2 == y1 + 1) ||
                (x2 == x1 - 1 && y2 == y1 - 1) ||
                (y2 == y1 + 1 && x2 == x1) ||
                (y2 == y1 - 1 && x2 == x1))
                return true;
            if (x2 == 2 && y2 == 7 && board.rights.q() && board.at(Position(1, 7)) == NONE &&
                board.at(Position(2, 7)) == NONE && board.at(Position(3, 7)) == NONE)
                return true;
            if (x2 == 6 && y2 == 7 && board.rights.k() && board.at(Position(5, 7)) == NONE &&
                board.at(Position(6, 7)) == NONE)
                return true;
            break;

        default:
            return false;
            break;
    }
    return false;
}

std::vector<Move> allLegalMoves(const Board& board)
{
    std::vector<Move> result;

    for (int x1 {0}; x1 < 8; ++x1)
        for (int y1 {0}; y1 < 8; ++y1)
            for (int x2 {0}; x2 < 8; ++x2)
                for (int y2 {0}; y2 < 8; ++y2)
                    if (moveIsLegal(Move(Position(x1, y1), Position(x2, y2)), board))
                        result.push_back(Move(Position(x1, y1), Position(x2, y2)));
    return result;
}

bool kingInCheck(Board board)
{
    board.toMove = !board.toMove;
    for (auto move : allLegalMoves(board))
        if (board.at(move.pos2).isKing())
            return true;
    return false;
}

std::vector<Move> slowLegalMoves(const Board& board)
{
    std::vector<Move> result;

    for (int x1 {0}; x1 < 8; ++x1)
        for (int y1 {0}; y1 < 8; ++y1)
            for (int x2 {0}; x2 < 8; ++x2)
                for (int y2 {0}; y2 < 8; ++y2)
                    if (moveIsLegal(Move(Position(x1, y1), Position(x2, y2)), board))
                    {
                        Board tempBoard(board);
                        quickMove(Move(Position(x1, y1), Position(x2, y2)), tempBoard);
                        if (!kingInCheck(tempBoard))
                            result.push_back(Move(Position(x1, y1), Position(x2, y2)));
                    }
    return result;
}

bool checkForStalemate(const Board& board)
{
    if (slowLegalMoves(board).empty())
        return true;
    return false;
}

bool checkForCheckmate(const Board& board)
{
    if (slowLegalMoves(board).empty() && kingInCheck(board))
        return true;
    return false;
}

void quickMove(Move move, Board& board)
{
    board.at(move.pos2) = board.at(move.pos1);
    board.at(move.pos1) = Piece(NONE);
}

void makeMove(Move move, Board& board)
{
    Piece& pieceToMove   = board.at(move.pos1);
    Piece& pieceToLandOn = board.at(move.pos2);
    const auto x1 = move.pos1.x();
    const auto y1 = move.pos1.y();
    const auto x2 = move.pos2.x();
    const auto y2 = move.pos2.y();

    if (board.toMove == BLACK)
        board.fullmoveClock++;

    board.halfmoveClock++;
    if (pieceToMove == PAWN_B || pieceToMove == PAWN_W || pieceToLandOn != NONE)
        board.halfmoveClock = 0;

    if (board.rights.enPassant())
    {
        if (pieceToMove == PAWN_W)
        {
            if (y1 == 4 && y2 == 5 && (x2 == x1 - 1 || x2 == x1 + 1))
                board.at(Position(x2, y2 - 1)) = NONE;
        }
        else if (pieceToMove == PAWN_B)
        {
            if (y1 == 3 && y2 == 2 && (x2 == x1 - 1 || x2 == x1 + 1))
                board.at(Position(x2, y2 + 1)) = NONE;
        }
    }

    if ((pieceToMove == PAWN_B && (y1 == 6 && y2 == 4)) ||
         (pieceToMove == PAWN_W && (y1 == 1 && y2 == 3)))
        board.rights.set_enPassant(static_cast<File>(x1));
    else
        board.rights.clear_enPassant();

    if (board.at(Position(0, 0)) != ROOK_W)
        board.rights.set_Q(0);
    if (board.at(Position(7, 0)) != ROOK_W)
        board.rights.set_K(0);
    if (board.at(Position(0, 7)) != ROOK_B)
        board.rights.set_q(0);
    if (board.at(Position(7, 7)) != ROOK_B)
        board.rights.set_k(0);

    if (x1 == 4 && y1 == 0 && x2 == 2 && y2 == 0 && board.rights.Q())
    {
        board.at(Position(0, 0)) = NONE;
        board.at(Position(3, 0)) = ROOK_W;
    }
    if (x1 == 4 && y1 == 0 && x2 == 6 && y2 == 0 && board.rights.K())
    {
        board.at(Position(7, 0)) = NONE;
        board.at(Position(5, 0)) = ROOK_W;
    }
    if (x1 == 4 && y1 == 7 && x2 == 2 && y2 == 7 && board.rights.q())
    {
        board.at(Position(0, 7)) = NONE;
        board.at(Position(3, 7)) = ROOK_B;
    }
    if (x1 == 4 && y1 == 7 && x2 == 6 && y2 == 7 && board.rights.k())
    {
        board.at(Position(0, 7)) = NONE;
        board.at(Position(5, 7)) = ROOK_B;
    }

    if (pieceToMove == KING_W)
    {
        board.rights.set_K(0);
        board.rights.set_Q(0);
    }
    if (pieceToMove == KING_B)
    {
        board.rights.set_k(0);
        board.rights.set_q(0);
    }

    pieceToLandOn = pieceToMove;
    pieceToMove = Piece(NONE);

    if (pieceToLandOn == PAWN_W && y2 == 7)
        pieceToLandOn = QUEEN_W;
    if (pieceToLandOn == PAWN_B && y2 == 0)
        pieceToLandOn = QUEEN_B;

    board.toMove = !board.toMove;

    if (checkForCheckmate(board))
        board.outcome = board.toMove ? WIN_FOR_WHITE : WIN_FOR_BLACK;
    else if (checkForStalemate(board))
        board.outcome = DRAWN;
    else if (board.halfmoveClock >= 100)
        board.outcome = DRAWN;
}
