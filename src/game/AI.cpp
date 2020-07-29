#include "game/AI.h"

#include "game/Move.h"
#ifdef DEBUG
    #include <cstdio>
#endif // DEBUG
#include <ctime>
#include <cmath>

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
                if (iy > 2 && ix > 1 && ix < 5)
                    temp *= 1.2f;
                if (iy > 2 && ix > 2 && ix < 5)
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
                if (iy < 5 && ix > 1 && ix < 5)
                    temp *= 1.2f;
                if (iy < 5 && ix > 2 && ix < 5)
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

Move bestMove(std::vector<std::vector<Tile>> tiles, int depth, bool player, CastlingRights CRsCopy, std::vector<bool> ePRsCopy)
{
    #if DEBUG_AI_SLOW_LEGALITY_CHECK == 1
        std::vector<Move> allMoves (getAvailableMoves(player, tiles, CRsCopy, ePRsCopy, 1));
    #else
        std::vector<Move> allMoves (getAvailableMoves(player, tiles, CRsCopy, ePRsCopy, 0));
    #endif // DEBUG_AI_SLOW_LEGALITY_CHECK
    srand(time(0));
    Move best = allMoves.at(floor((static_cast<float>(rand()) / RAND_MAX) * allMoves.size()));
    #if DEBUG_AI_RANDOM == 1
        return best;
    #endif // DEBUG_AI_RANDOM

    float bestPosition = ((!player)*2 - 1) * 999999;

    #ifdef DEBUG
    if (depth == 1)
        fprintf(stderr, "DEPTH = %d\n", depth);
    #endif // DEBUG
        for (auto move : allMoves)
        {
            std::vector<std::vector<Tile>> newPosition = tiles;
            makeMove(move, newPosition, CRsCopy, ePRsCopy);
            int d = depth;
            int p = player;
            while (d <= MAX_DEPTH)
            {
                p = !p;
                makeMove(bestMove(newPosition, d + 1, p, CRsCopy, ePRsCopy), newPosition, CRsCopy, ePRsCopy);
                d++;
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
            if (depth == 1)
                fprintf(stderr, "Possible move for %d = %d, %d to %d, %d  Expected rating change = %f\n",
                        player, move.pos1X, move.pos1Y, move.pos2X, move.pos2Y, positionRating(newPosition) - positionRating(tiles));
            #endif //DEBUG
        }
    #ifdef DEBUG
    if (depth == 1)
        fprintf(stderr, "\n");
    #endif // DEBUG
    return best;
}
