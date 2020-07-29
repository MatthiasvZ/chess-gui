#include "game/Move.h"

bool moveIsLegal(Move move, std::vector<std::vector<Tile>>& tiles, bool whoToMove, const CastlingRights& CRs, const std::vector<bool>& ePRs)
{
    bool pathIsClear;
    Piece pieceToMove {tiles.at(move.pos1X - 1).at(move.pos1Y - 1).getPiece()};
    Piece pieceToLandOn {tiles.at(move.pos2X - 1).at(move.pos2Y - 1).getPiece()};

    if (move.pos1X == move.pos2X && move.pos1Y == move.pos2Y)
        return 0;
    if (isTeammate(pieceToMove, pieceToLandOn))
        return 0;

    switch (pieceToMove)
    {
    case Piece::pawn_w:
        if (!whoToMove)
            return false;
        if ((move.pos2X == move.pos1X + 1 || move.pos2X == move.pos1X - 1) &&
            move.pos2Y == move.pos1Y + 1 && pieceToLandOn != Piece::none)
            return true;
        if (move.pos1Y == 5 && move.pos2Y == 6 && (move.pos2X == move.pos1X - 1 || move.pos2X == move.pos1X + 1))
            if (ePRs.at(move.pos2X - 1))
                return true;
        if (move.pos2X != move.pos1X || tiles.at(move.pos2X - 1).at(move.pos2Y - 1).getPiece() != Piece::none)
            return false;
        if (move.pos2Y == move.pos1Y + 1)
            return true;
        if (move.pos1Y == 2 && move.pos2Y == 4 &&
            tiles.at(move.pos1X - 1).at(2).getPiece() == Piece::none)
            return true;
        break;
    case Piece::pawn_b:
        if (whoToMove)
            return false;
        if ((move.pos2X == move.pos1X + 1 || move.pos2X == move.pos1X - 1) &&
            move.pos2Y == move.pos1Y - 1 && pieceToLandOn != Piece::none)
            return true;
        if (move.pos1Y == 4 && move.pos2Y == 3 && ((move.pos2X == move.pos1X - 1) || (move.pos2X == move.pos1X + 1)))
            if (ePRs.at(move.pos2X - 1))
                return true;
        if (move.pos2X != move.pos1X || tiles.at(move.pos2X - 1).at(move.pos2Y - 1).getPiece() != Piece::none)
            return false;
        if (move.pos2Y == move.pos1Y - 1)
            return true;
        if (move.pos1Y == 7 && move.pos2Y == 5 &&
            tiles.at(move.pos1X - 1).at(5).getPiece() == Piece::none)
            return true;
        break;

    case Piece::knight_w:
        if (!whoToMove)
            return false;
        [[fallthrough]];
    case Piece::knight_b:
        if (whoToMove && pieceToMove == knight_b)
            return false;
        if (move.pos2X == move.pos1X + 2 && move.pos2Y == move.pos1Y + 1)
            return true;
        if (move.pos2X == move.pos1X + 2 && move.pos2Y == move.pos1Y - 1)
            return true;
        if (move.pos2X == move.pos1X - 2 && move.pos2Y == move.pos1Y + 1)
            return true;
        if (move.pos2X == move.pos1X - 2 && move.pos2Y == move.pos1Y - 1)
            return true;
        if (move.pos2Y == move.pos1Y + 2 && move.pos2X == move.pos1X + 1)
            return true;
        if (move.pos2Y == move.pos1Y + 2 && move.pos2X == move.pos1X - 1)
            return true;
        if (move.pos2Y == move.pos1Y - 2 && move.pos2X == move.pos1X + 1)
            return true;
        if (move.pos2Y == move.pos1Y - 2 && move.pos2X == move.pos1X - 1)
            return true;
        break;

    case Piece::bishop_w:
        if (!whoToMove)
            return false;
        [[fallthrough]];
    case Piece::bishop_b:
        if (whoToMove && pieceToMove == Piece::bishop_b)
            return false;
        pathIsClear = true;
        if (move.pos1X - move.pos1Y == move.pos2X - move.pos2Y)
        {
            for (int ix {move.pos1X + 1}, iy {move.pos1Y + 1}; ix < move.pos2X; ix++, iy++)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
            for (int ix {move.pos1X - 1}, iy {move.pos1Y - 1}; ix > move.pos2X; ix--, iy--)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
        }
        if (move.pos1X + move.pos1Y == move.pos2X + move.pos2Y)
        {
            for (int ix {move.pos1X + 1}, iy {move.pos1Y - 1}; ix < move.pos2X; ix++, iy--)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
            for (int ix {move.pos1X - 1}, iy {move.pos1Y + 1}; ix > move.pos2X; ix--, iy++)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
        }
        if ((move.pos1X - move.pos1Y == move.pos2X - move.pos2Y) && pathIsClear) // '/'
            return true;
        if ((move.pos1X + move.pos1Y == move.pos2X + move.pos2Y) && pathIsClear) // '\'
            return true;
        break;

    case Piece::rook_w:
        if (!whoToMove)
            return false;
        [[fallthrough]];
    case Piece::rook_b:
        if (whoToMove && pieceToMove == rook_b)
            return false;
        pathIsClear = true;
        for (int ix {move.pos1X + 1}; ix < move.pos2X; ix++)
            if (tiles.at(ix - 1).at(move.pos1Y - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int ix {move.pos1X - 1}; ix > move.pos2X; ix--)
            if (tiles.at(ix - 1).at(move.pos1Y - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int iy {move.pos1Y + 1}; iy < move.pos2Y; iy++)
            if (tiles.at(move.pos1X - 1).at(iy - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int iy {move.pos1Y - 1}; iy > move.pos2Y; iy--)
            if (tiles.at(move.pos1X - 1).at(iy - 1).getPiece() != Piece::none)
                pathIsClear = false;
        if ((move.pos1X == move.pos2X || move.pos1Y == move.pos2Y) && pathIsClear)
            return true;
        break;

    case Piece::queen_w:
        if (!whoToMove)
            return false;
        [[fallthrough]];
    case Piece::queen_b:
        if (whoToMove && pieceToMove == queen_b)
            return false;
        pathIsClear = true;
        if (move.pos1X == move.pos2X || move.pos1Y == move.pos2Y)
        {
        for (int ix {move.pos1X + 1}; ix < move.pos2X; ix++)
            if (tiles.at(ix - 1).at(move.pos1Y - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int ix {move.pos1X - 1}; ix > move.pos2X; ix--)
            if (tiles.at(ix - 1).at(move.pos1Y - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int iy {move.pos1Y + 1}; iy < move.pos2Y; iy++)
            if (tiles.at(move.pos1X - 1).at(iy - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int iy {move.pos1Y - 1}; iy > move.pos2Y; iy--)
            if (tiles.at(move.pos1X - 1).at(iy - 1).getPiece() != Piece::none)
                pathIsClear = false;
        }
        if (move.pos1X - move.pos1Y == move.pos2X - move.pos2Y)
        {
            for (int ix {move.pos1X + 1}, iy {move.pos1Y + 1}; ix < move.pos2X; ix++, iy++)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
            for (int ix {move.pos1X - 1}, iy {move.pos1Y - 1}; ix > move.pos2X; ix--, iy--)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
        }
        if (move.pos1X + move.pos1Y == move.pos2X + move.pos2Y)
        {
            for (int ix {move.pos1X + 1}, iy {move.pos1Y - 1}; ix < move.pos2X; ix++, iy--)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
            for (int ix {move.pos1X - 1}, iy {move.pos1Y + 1}; ix > move.pos2X; ix--, iy++)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
        }
        if (move.pos1X - move.pos1Y == move.pos2X - move.pos2Y && pathIsClear)
            return true;
        if (move.pos1X + move.pos1Y == move.pos2X + move.pos2Y && pathIsClear)
            return true;
        if ((move.pos1X == move.pos2X || move.pos1Y == move.pos2Y) && pathIsClear)
            return true;
        break;

    case Piece::king_w:
        if (!whoToMove)
            return false;
        if (move.pos2X == 3 && move.pos2Y == 1 && CRs.whiteCanCastleQs && tiles.at(1).at(0).getPiece() == Piece::none &&
            tiles.at(2).at(0).getPiece() == Piece::none &&
            tiles.at(3).at(0).getPiece() == Piece::none &&
            tiles.at(0).at(0).getPiece() == Piece::rook_w)
            return true;
        if (move.pos2X == 7 && move.pos2Y == 1 && CRs.whiteCanCastleKs && tiles.at(5).at(0).getPiece() == Piece::none &&
            tiles.at(6).at(0).getPiece() == Piece::none &&
            tiles.at(7).at(0).getPiece() == Piece::rook_w)
            return true;
        [[fallthrough]];
    case Piece::king_b:
        if (whoToMove && pieceToMove == king_b)
            return false;
        if ((move.pos2X == move.pos1X + 1 && move.pos2Y == move.pos1Y) ||
            (move.pos2X == move.pos1X + 1 && move.pos2Y == move.pos1Y + 1) ||
            (move.pos2X == move.pos1X + 1 && move.pos2Y == move.pos1Y - 1) ||
            (move.pos2X == move.pos1X - 1 && move.pos2Y == move.pos1Y) ||
            (move.pos2X == move.pos1X - 1 && move.pos2Y == move.pos1Y + 1) ||
            (move.pos2X == move.pos1X - 1 && move.pos2Y == move.pos1Y - 1) ||
            (move.pos2Y == move.pos1Y + 1 && move.pos2X == move.pos1X) ||
            (move.pos2Y == move.pos1Y - 1 && move.pos2X == move.pos1X))
            return true;
        if (pieceToMove == king_b && move.pos2X == 3 && move.pos2Y == 8 && CRs.blackCanCastleQs &&
            tiles.at(1).at(7).getPiece() == Piece::none &&
            tiles.at(2).at(7).getPiece() == Piece::none &&
            tiles.at(3).at(7).getPiece() == Piece::none &&
            tiles.at(0).at(7).getPiece() == Piece::rook_b)
            return true;
        if (pieceToMove == king_b && move.pos2X == 7 && move.pos2Y == 8 && CRs.blackCanCastleKs &&
            tiles.at(5).at(7).getPiece() == Piece::none &&
            tiles.at(6).at(7).getPiece() == Piece::none &&
            tiles.at(7).at(7).getPiece() == Piece::rook_b)
            return true;
    default:
        return false;
        break;
    }

    return false;
}

std::vector<Move> getAvailableMoves(bool whoToMove, std::vector<std::vector<Tile>> tiles, const CastlingRights& CRs, const std::vector<bool>& ePRs)
{
    std::vector<Move> availableMoves;
    if (whoToMove == 1)
    {
        for (int ix {0}; ix < 8; ix++)
            for (int iy {0}; iy < 8; iy++)
            {
                if (tiles.at(ix).at(iy).getPiece() == Piece::bishop_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::king_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::knight_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::pawn_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::queen_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::rook_w)
                {
                    for (int jx {0}; jx < 8; jx++)
                        for (int jy {0}; jy < 8; jy++)
                        {

                            if (moveIsLegal(Move(ix + 1, iy + 1, jx + 1, jy + 1), tiles, whoToMove, CRs, ePRs))
                                availableMoves.push_back(Move(ix + 1, iy + 1, jx + 1, jy + 1));
                        }
                }
            }
    }
    else
    {
        for (int ix {0}; ix < 8; ix++)
            for (int iy {0}; iy < 8; iy++)
            {
                if (tiles.at(ix).at(iy).getPiece() == Piece::bishop_b ||
                    tiles.at(ix).at(iy).getPiece() == Piece::king_b ||
                    tiles.at(ix).at(iy).getPiece() == Piece::knight_b ||
                    tiles.at(ix).at(iy).getPiece() == Piece::pawn_b ||
                    tiles.at(ix).at(iy).getPiece() == Piece::queen_b ||
                    tiles.at(ix).at(iy).getPiece() == Piece::rook_b)
                {
                    for (int jx {0}; jx < 8; jx++)
                        for (int jy {0}; jy < 8; jy++)
                        {

                            if (moveIsLegal(Move(ix + 1, iy + 1, jx + 1, jy + 1), tiles, whoToMove, CRs, ePRs))
                                availableMoves.push_back(Move(ix + 1, iy + 1, jx + 1, jy + 1));
                        }
                }
            }
    }
    return availableMoves;
}

void makeMove(Move move, std::vector<std::vector<Tile>>& tiles, CastlingRights& CRs, std::vector<bool>& ePRs)
{
    Piece pieceToMove {tiles.at(move.pos1X - 1).at(move.pos1Y - 1).getPiece()};

    // Castling
    if (pieceToMove == king_w)
    {
        CRs.whiteCanCastleKs = false;
        CRs.whiteCanCastleQs = false;
    }

    if (pieceToMove == king_b)
    {
        CRs.blackCanCastleKs = false;
        CRs.blackCanCastleQs = false;
    }

    if ((pieceToMove == king_w && move.pos2X == 3 && move.pos2Y == 1) || (pieceToMove == king_w && move.pos2X == 7 && move.pos2Y == 1))
    {
        CRs.whiteCanCastleKs = false;
        CRs.whiteCanCastleQs = false;
    }

    if ((pieceToMove == king_b && move.pos2X == 3 && move.pos2Y == 8) || (pieceToMove == king_b && move.pos2X == 7 && move.pos2Y == 8))
    {
        CRs.blackCanCastleKs = false;
        CRs.blackCanCastleQs = false;
    }
    if (move.pos1X == 1 && move.pos1Y == 1)
        CRs.whiteCanCastleQs = false;
    if (move.pos1X == 8 && move.pos1Y == 1)
        CRs.whiteCanCastleKs = false;
    if (move.pos1X == 1 && move.pos1Y == 8)
        CRs.blackCanCastleQs = false;
    if (move.pos1X == 8 && move.pos1Y == 8)
        CRs.blackCanCastleKs = false;

    if (pieceToMove == king_w && move.pos1X == 5 && move.pos1Y == 1 && move.pos2X == 3 && move.pos2Y == 1)
    {
        tiles.at(0).at(0).setPiece(Piece::none);
        tiles.at(3).at(0).setPiece(Piece::rook_w);
    }

    if (pieceToMove == king_w && move.pos1X == 5 && move.pos1Y == 1 && move.pos2X == 7 && move.pos2Y == 1)
    {
        tiles.at(7).at(0).setPiece(Piece::none);
        tiles.at(5).at(0).setPiece(Piece::rook_w);
    }

    if (pieceToMove == king_b && move.pos1X == 5 && move.pos1Y == 8 && move.pos2X == 3 && move.pos2Y == 8)
    {
        tiles.at(0).at(7).setPiece(Piece::none);
        tiles.at(3).at(7).setPiece(Piece::rook_b);
    }

    if (pieceToMove == king_b && move.pos1X == 5 && move.pos1Y == 8 && move.pos2X == 7 && move.pos2Y == 8)
    {
        tiles.at(7).at(7).setPiece(Piece::none);
        tiles.at(5).at(7).setPiece(Piece::rook_b);

    }

    // En Passant
    if (pieceToMove == pawn_w && move.pos1Y == 5 && move.pos2Y == 6 && move.pos2X != move.pos1X)
        if (ePRs.at(move.pos2X - 1))
            tiles.at(move.pos2X - 1).at(move.pos2Y - 2).setPiece(Piece::none);

    if (pieceToMove == pawn_b && move.pos1Y == 4 && move.pos2Y == 3 && move.pos2X != move.pos1X)
        if (ePRs.at(move.pos2X - 1))
            tiles.at(move.pos2X - 1).at(move.pos2Y).setPiece(Piece::none);

    if ((pieceToMove == pawn_w && move.pos1Y == 2 && move.pos2Y == 4) || (pieceToMove == pawn_b && move.pos1Y == 7 && move.pos2Y == 5))
        ePRs.at(move.pos1X - 1) = true;
    else
        ePRs.assign({false, false, false, false, false, false, false, false});

    // Actually execute the move
    tiles.at(move.pos2X - 1).at(move.pos2Y - 1).setPiece(
            tiles.at(move.pos1X - 1).at(move.pos1Y - 1).getPiece());
    tiles.at(move.pos1X - 1).at(move.pos1Y - 1).setPiece(Piece::none);

    // Queening
    for (int ix {0}; ix < 8; ix++)
    {
    if (tiles.at(ix).at(0).getPiece() == Piece::pawn_b)
        tiles.at(ix).at(0).setPiece(Piece::queen_b);
    if (tiles.at(ix).at(7).getPiece() == Piece::pawn_w)
        tiles.at(ix).at(7).setPiece(Piece::queen_w);
    }
}
