#include "other/FileManagement.h"
#include "gl/Window.h"
#include "gl/Shader.h"
#include "gl/Texture.h"
#include "game/Tile.h"

#ifdef DEBUG
#include "other/ErrorFeedback.h"
#endif //DEBUG

#include <iostream>
#include <vector>

#define PLAY_AS 0 // 1 = white, 0 = black
#define PLAY_AGAINST_AI true
#define AI_VS_AI false
#define MAX_DEPTH 3 // must be odd?

namespace
{
Window window;
bool gameOver {false};
}

struct Move
{
    int pos1X;
    int pos1Y;
    int pos2X;
    int pos2Y;
};

struct CastlingRights
{
    bool whiteCanCastleKs {true};
    bool whiteCanCastleQs {true};
    bool blackCanCastleKs {true};
    bool blackCanCastleQs {true};
};

inline bool isTeammate(Piece ownPiece, Piece pieceInQ)
{
    if (pieceInQ == Piece::none)
        return false;
    if ((ownPiece - pieceInQ) % 2 == 0)
        return true;
    return false;
}
bool moveIsLegal(Move move, std::vector<std::vector<Tile>>& tiles, bool whoToMove, const CastlingRights& CRs)
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

std::vector<Move> getAvailableMoves(bool whoToMove, std::vector<std::vector<Tile>> tiles, const CastlingRights& CRs)
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

                            if (moveIsLegal(Move(ix + 1, iy + 1, jx + 1, jy + 1), tiles, whoToMove, CRs))
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

                            if (moveIsLegal(Move(ix + 1, iy + 1, jx + 1, jy + 1), tiles, whoToMove, CRs))
                                availableMoves.push_back(Move(ix + 1, iy + 1, jx + 1, jy + 1));
                        }
                }
            }
    }
    return availableMoves;
}

float positionRating(const std::vector<std::vector<Tile>>& tiles)
{
    constexpr float PAWN {0.7f};
    constexpr float KNIGHT {3.0f};
    constexpr float BISHOP {3.1f};
    constexpr float ROOK {5.0f};
    constexpr float QUEEN {9.0f};
    constexpr float KING {9999.0f};

    float rating {0.0f};
    float temp;
    bool passed;

    for (int ix {0}; ix < 8; ix++)
        for (int iy {0}; iy < 8; iy++)
        {
            switch (tiles.at(ix).at(iy).getPiece())
            {
            case Piece::pawn_w:
                temp = PAWN;

                passed = true;

                for (int jy {0}; jy < 8; jy++)
                {
                    // punish doubled/tripled/etc. pawns
                    if (tiles.at(ix).at(jy).getPiece() == pawn_w && iy != jy)
                        temp /= 1.3f;
                    // reward non-isolated pawns
                    if (ix > 0 ? tiles.at(ix - 1).at(jy).getPiece() == pawn_w : true)
                        temp *= 1.15f;
                    if (ix < 7 ? tiles.at(ix + 1).at(jy).getPiece() == pawn_w : true)
                        temp *= 1.15f;
                    // reward passed pawns
                    if ( (tiles.at(ix).at(jy).getPiece() == pawn_b ||
                         (ix < 7 ? tiles.at(ix + 1).at(jy).getPiece() == pawn_b : true) ||
                         (ix > 0 ? tiles.at(ix - 1).at(jy).getPiece() == pawn_b : true)) &&
                         jy > iy)
                         passed = false;
                }
                if (passed)
                    temp *= 2.0f;

                // reward centralising centre pawns (except for the f-pawn)
                if (iy > 2 && ix > 2 && ix < 5)
                    temp *= 1.2f;
                if (iy > 2 && ix > 3 && ix < 5)
                    temp *= 1.1f;

                // value centre pawns over those near the edge
                if (ix > 2 && ix < 6)
                    temp *= 1.2f;
                if (ix > 3 && ix < 5)
                    temp *= 1.1f;

                rating += temp;
                break;
            case Piece::pawn_b:
                temp = PAWN;
                passed = true;

                for (int jy {0}; jy < 8; jy++)
                {
                    // punish doubled/tripled/etc. pawns
                    if (tiles.at(ix).at(jy).getPiece() == pawn_b && iy != jy)
                        temp /= 1.3f;
                    // reward non-isolated pawns
                    if (ix > 0 ? tiles.at(ix - 1).at(jy).getPiece() == pawn_b : true)
                        temp *= 1.15f;
                    if (ix < 7 ? tiles.at(ix + 1).at(jy).getPiece() == pawn_b : true)
                        temp *= 1.15f;
                    // reward passed pawns
                    if ( (tiles.at(ix).at(jy).getPiece() == pawn_w ||
                         (ix < 7 ? tiles.at(ix + 1).at(jy).getPiece() == pawn_w : true) ||
                         (ix > 0 ? tiles.at(ix - 1).at(jy).getPiece() == pawn_w : true)) &&
                         jy < iy)
                         passed = false;
                }
                if (passed)
                    temp *= 2.0f;

                // reward centralising centre pawns (except for the f-pawn)
                if (iy > 5 && ix > 2 && ix < 5)
                    temp *= 1.2f;
                if (iy > 5 && ix > 3 && ix < 5)
                    temp *= 1.1f;

                // value centre pawns over those near the edge
                if (ix > 2 && ix < 6)
                    temp *= 1.2f;
                if (ix > 3 && ix < 5)
                    temp *= 1.1f;

                rating -= temp;
                break;
            case Piece::knight_w:
                temp = KNIGHT;

                rating += temp;
                break;
            case Piece::knight_b:
                rating -= KNIGHT;
                break;
            case Piece::bishop_w:
                rating += BISHOP;
                break;
            case Piece::bishop_b:
                rating -= BISHOP;
                break;
            case Piece::rook_w:
                rating += ROOK;
                break;
            case Piece::rook_b:
                rating -= ROOK;
                break;
            case Piece::queen_w:
                rating += QUEEN;
                break;
            case Piece::queen_b:
                rating -= QUEEN;
                break;
            case Piece::king_w:
                rating += KING;
                break;
            case Piece::king_b:
                rating -= KING;
                break;

            default:
                break;
            }
        }
    return rating;
}

void makeMove(Move move, std::vector<std::vector<Tile>>& tiles, CastlingRights& CRs)
{
    Piece pieceToMove {tiles.at(move.pos1X - 1).at(move.pos1Y - 1).getPiece()};

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

    tiles.at(move.pos2X - 1).at(move.pos2Y - 1).setPiece(
            tiles.at(move.pos1X - 1).at(move.pos1Y - 1).getPiece());
    tiles.at(move.pos1X - 1).at(move.pos1Y - 1).setPiece(Piece::none);

    for (int ix {0}; ix < 8; ix++)
    {
    if (tiles.at(ix).at(0).getPiece() == Piece::pawn_b)
        tiles.at(ix).at(0).setPiece(Piece::queen_b);
    if (tiles.at(ix).at(7).getPiece() == Piece::pawn_w)
        tiles.at(ix).at(7).setPiece(Piece::queen_w);
    }
}

Move bestMove(std::vector<std::vector<Tile>> tiles, int depth, bool player, CastlingRights CRsCopy)
{
    std::vector<Move> allMoves (getAvailableMoves(player, tiles, CRsCopy));
    // srand(time(0));
    // Move best = allMoves.at(floor((static_cast<float>(rand()) / RAND_MAX) * allMoves.size()));
    Move best = allMoves.at(0);

    float bestPosition = ((!player)*2 - 1) * 999999;

    #ifdef DEBUG
    if (depth == MAX_DEPTH)
        fprintf(stderr, "DEPTH = %d\n", depth);
    #endif // DEBUG
        for (auto move : allMoves)
        {
            std::vector<std::vector<Tile>> newPosition = tiles;
            makeMove(move, newPosition, CRsCopy);
            int d = depth;
            while (d > 1)
            {
                makeMove(bestMove(newPosition, d - 1, (window.getPlayer() + d) % 2, CRsCopy), newPosition, CRsCopy);
                d--;
            }

            if (player == 1)
                if (positionRating(newPosition) > bestPosition)
                {
                    best = move;
                    bestPosition = positionRating(newPosition);
                }
            if (player == 0)
                if (positionRating(newPosition) < bestPosition)
                {
                    best = move;
                    bestPosition = positionRating(newPosition);
                }
            #ifdef DEBUG
            if (depth == MAX_DEPTH)
                fprintf(stderr, "Possible move for %d = %d, %d to %d, %d  Expected rating change = %f\n",
                        player, move.pos1X, move.pos1Y, move.pos2X, move.pos2Y, positionRating(newPosition) - positionRating(tiles));
            #endif //DEBUG
        }
    #ifdef DEBUG
    if (depth == MAX_DEPTH)
        fprintf(stderr, "\n");
    #endif // DEBUG
    return best;
}

void updateGame(std::vector<std::vector<Tile>>& tiles)
{
    bool whiteKingFound {false};
    bool blackKingFound {false};
    bool allPawnsGone {true};

    for (int ix {0}; ix < 8; ix++)
    {
        for (int iy {0}; iy < 8; iy++)
        {
            // End the game if the king was captured
            if (tiles.at(ix).at(iy).getPiece() == Piece::king_w)
                whiteKingFound = true;
            if (tiles.at(ix).at(iy).getPiece() == Piece::king_b)
                blackKingFound = true;

            if (tiles.at(ix).at(iy).getPiece() == Piece::pawn_w ||
                tiles.at(ix).at(iy).getPiece() == Piece::pawn_w)
                allPawnsGone = false;
        }
    }
    if (!whiteKingFound)
    {
        window.setGameOver(0);
        gameOver = true;
    }
    if (!blackKingFound)
    {
        window.setGameOver(1);
        gameOver = true;
    }
    if (allPawnsGone)
    {
        unsigned char bishopCount_W {0};
        unsigned char bishopCount_B {0};
        unsigned char knightCount_W {0};
        unsigned char knightCount_B {0};
        bool majorPieceFound {false};

        for (int ix {0}; ix < 8; ix++)
            for (int iy {0}; iy < 8; iy++)
            {
                if (tiles.at(ix).at(iy).getPiece() == Piece::bishop_w)
                    bishopCount_W++;
                if (tiles.at(ix).at(iy).getPiece() == Piece::bishop_b)
                    bishopCount_B++;
                if (tiles.at(ix).at(iy).getPiece() == Piece::knight_w)
                    knightCount_W++;
                if (tiles.at(ix).at(iy).getPiece() == Piece::knight_b)
                    knightCount_B++;
                if (tiles.at(ix).at(iy).getPiece() == Piece::rook_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::rook_b ||
                    tiles.at(ix).at(iy).getPiece() == Piece::queen_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::queen_b)
                    majorPieceFound = true;
            }
        if (majorPieceFound == false &&
            bishopCount_W < 2 && bishopCount_B < 2 &&
            knightCount_W < 3 && knightCount_B < 3 &&
            !(bishopCount_W == 1 && knightCount_W > 0) &&
            !(bishopCount_B == 1 && knightCount_B > 0))
        {
            gameOver = true;
            window.setGameOver(2);
        }
    }
    fprintf(stderr, "Rating = %f\n", positionRating(tiles));
}

int main()
{
    FileManagement::setWorkingDir();
    FileManagement::createFolders();

    SourcePackage srcpkg(Shader::readFromFile("src/shaders/BasicTexVertex.glsl"), \
                         Shader::readFromFile("src/shaders/BasicTexFragment.glsl"));
    Shader shader(srcpkg);
    shader.bindShader();
    //Texture atlas("assets/texAtlas.bmp", 0);
    //shader.setUniform1i("tex", 0);

    Renderer renderer;

    IndexBuffer ibo;
    VertexBufferLayout layout;

    unsigned int indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);

    ibo.addData(indices, sizeof(indices) / sizeof(*indices));

    std::vector<std::vector<VertexArray>> vaos;
    std::vector<std::vector<VertexBuffer>> vbos;
    std::vector<std::vector<Tile>> tiles;

    vaos.reserve(8);
    vbos.reserve(8);
    tiles.reserve(8);
    for (int ix {0}; ix < 8; ix++)
    {
        vaos.push_back(std::vector<VertexArray>());
        vbos.push_back(std::vector<VertexBuffer>());
        tiles.push_back(std::vector<Tile>());
        vaos.at(ix).reserve(8);
        vbos.at(ix).reserve(8);
        tiles.at(ix).reserve(8);

        for (int iy {0}; iy < 8; iy++)
        {
            tiles.at(ix).push_back(Tile(ix + 1, iy + 1));
            vaos.at(ix).push_back(VertexArray());
            vaos.at(ix).at(iy).genArray();
            vbos.at(ix).push_back(VertexBuffer());
            vbos.at(ix).at(iy).addData(tiles.at(ix).at(iy).getVertices().data(), 16 * sizeof(GL_FLOAT));
            vaos.at(ix).at(iy).addBuffer(vbos.at(ix).at(iy), layout);
        }
    }

    float boardVertices[] =
    {// X       Y     U     V
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f
    };

    VertexArray boardVao;
    VertexBuffer boardVbo;
    boardVao.genArray();
    boardVbo.addData(boardVertices, 16 * sizeof(GL_FLOAT));
    boardVao.addBuffer(boardVbo, layout);

    Texture chequerboard("assets/chequerboard.bmp", 0);
    Texture pawn_W("assets/pawn_white.png", 1);
    Texture pawn_B("assets/pawn_black.png", 2);
    Texture knight_W("assets/knight_white.png", 3);
    Texture knight_B("assets/knight_black.png", 4);
    Texture bishop_W("assets/bishop_white.png", 5);
    Texture bishop_B("assets/bishop_black.png", 6);
    Texture rook_W("assets/rook_white.png", 7);
    Texture rook_B("assets/rook_black.png", 8);
    Texture queen_W("assets/queen_white.png", 9);
    Texture queen_B("assets/queen_black.png", 10);
    Texture king_W("assets/king_white.png", 11);
    Texture king_B("assets/king_black.png", 12);
    Texture selection("assets/selection.png", 13);

    int tileX {0};
    int tileY {0};
    int selectedTileX {0};
    int selectedTileY {0};
    CastlingRights CRs;

    while (window.shouldRun())
    {
        renderer.clear();

        shader.setUniform1i("tex", 0);
        renderer.drawVA(boardVao, ibo);

        // Selecting/Moving the pieces
        window.getSelectedTile(tileX, tileY);
        if ((window.getPlayer() == PLAY_AS || !PLAY_AGAINST_AI) && !AI_VS_AI)
        {
            if (!selectedTileX)
            {
                if (!tileX)
                    goto postTileSelection;
                if (tiles.at(tileX - 1).at(tileY - 1).getPiece() == Piece::none)
                    goto postTileSelection;
                selectedTileX = tileX;
                selectedTileY = tileY;
                window.removeSelection();
            }
            else if (tileX)
            {
                Move move(selectedTileX, selectedTileY, tileX, tileY);
                if (moveIsLegal(move, tiles, window.getPlayer(), CRs))
                {
                    makeMove(move, tiles, CRs);
                    updateGame(tiles);
                    if (gameOver)
                        goto gameOver;
                    window.setplayer(!window.getPlayer());
                    window.updateTitle();
                }
                selectedTileX = 0;
                selectedTileY = 0;
                window.removeSelection();
            }
            postTileSelection: // Skip ifs, if the tile is empty

            // Draw the selection underlay
            if (selectedTileX)
            {
                shader.setUniform1i("tex", 13);
                renderer.drawVA(vaos.at(selectedTileX - 1).at(selectedTileY - 1), ibo);
            }
        }
        else // Have the AI play
        {
            makeMove(bestMove(tiles, MAX_DEPTH, window.getPlayer(), CRs), tiles, CRs);
            updateGame(tiles);
            if (gameOver)
                goto gameOver;
            window.setplayer(!window.getPlayer());
            window.updateTitle();
        }

        // Draw the pieces
        for (int ix {0}; ix < 8; ix++)
            for (int iy {0}; iy < 8; iy++)
            {
                if (!tiles.at(ix).at(iy).getPiece())
                    continue;
                shader.setUniform1i("tex", tiles.at(ix).at(iy).getPiece());
                renderer.drawVA(vaos.at(ix).at(iy), ibo);
            }

        window.countfps();
        window.updateWindow();
        #ifdef DEBUG
            ErrorFeedback::getErrors();
        #endif //DEBUG
    }

    gameOver:
    while (window.shouldRun())
    {
        window.updateWindow();
        #ifdef DEBUG
            ErrorFeedback::getErrors();
        #endif //DEBUG
    }
    return 0;
}
