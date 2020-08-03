#include "game/AI.h"

#include "game/Move.h"
#ifdef DEBUG
    #include <cstdio>
#endif // DEBUG
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cassert>

float positionRating(const std::vector<std::vector<Tile>>& tiles)
{
    constexpr float PAWN {0.8f};
    constexpr float KNIGHT {3.0f};
    constexpr float BISHOP {3.1f};
    constexpr float ROOK {5.0f};
    constexpr float QUEEN {9.0f};
    constexpr float KING {9999.0f};

    float rating {0.0f};
    float temp;
    #if DEBUG_AI_VALUE_MATERIAL_ONLY == 0
        bool passed;
    #endif // DEBUG_AI_VALUE_MATERIAL_ONLY

    for (int ix {0}; ix < 8; ix++)
        for (int iy {0}; iy < 8; iy++)
        {
            switch (tiles.at(ix).at(iy).getPiece())
            {
            case Piece::pawn_w:
                temp = PAWN;

                #if DEBUG_AI_VALUE_MATERIAL_ONLY == 0
                passed = true;
                for (int jy {0}; jy < 8; jy++)
                {
                    // punish doubled/tripled/etc. pawns
                    if (tiles.at(ix).at(jy).getPiece() == pawn_w && iy != jy)
                        temp /= 1.2f;
                    // reward non-isolated pawns
                    if (ix > 0 ? tiles.at(ix - 1).at(jy).getPiece() == pawn_w : true)
                        temp *= 1.1f;
                    if (ix < 7 ? tiles.at(ix + 1).at(jy).getPiece() == pawn_w : true)
                        temp *= 1.1f;
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
                    temp *= 1.1f;
                if (iy > 2 && ix > 2 && ix < 5)
                    temp *= 1.05f;

                // value centre pawns over those near the edge
                if (ix > 2 && ix < 6)
                    temp *= 1.1f;
                if (ix > 3 && ix < 5)
                    temp *= 1.05f;
                #endif // DEBUG_AI_VALUE_MATERIAL_ONLY

                rating += temp;
                break;
            case Piece::pawn_b:
                temp = PAWN;

                #if DEBUG_AI_VALUE_MATERIAL_ONLY == 0
                passed = true;
                for (int jy {0}; jy < 8; jy++)
                {
                    // punish doubled/tripled/etc. pawns
                    if (tiles.at(ix).at(jy).getPiece() == pawn_b && iy != jy)
                        temp /= 1.2f;
                    // reward non-isolated pawns
                    if (ix > 0 ? tiles.at(ix - 1).at(jy).getPiece() == pawn_b : true)
                        temp *= 1.1f;
                    if (ix < 7 ? tiles.at(ix + 1).at(jy).getPiece() == pawn_b : true)
                        temp *= 1.1f;
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
                    temp *= 1.1f;
                if (iy < 5 && ix > 2 && ix < 5)
                    temp *= 1.05f;

                // value centre pawns over those near the edge
                if (ix > 2 && ix < 6)
                    temp *= 1.1f;
                if (ix > 3 && ix < 5)
                    temp *= 1.05f;
                #endif // DEBUG_AI_VALUE_MATERIAL_ONLY

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

bool positionIsSafe(const std::vector<std::vector<Tile>>& tiles, const std::vector<Move>& allMoves)
{
    for (auto move : allMoves)
        if (tiles[move.pos1X - 1][move.pos1Y - 1].getPiece() < tiles[move.pos2X - 1][move.pos2Y - 1].getPiece())
            return 0;
    return 1;

}

inline bool moveIsCapture(const std::vector<std::vector<Tile>>& tiles, const Move& move)
{
    if (tiles.at(move.pos2X - 1).at(move.pos2Y - 1).getPiece() != Piece::none)
        return 1;
    return 0;
}

Move bestMove(const std::vector<std::vector<Tile>>& tiles, int depth, bool maximisingPlayer, const CastlingRights& CRs, const std::vector<bool>& ePRs, float α, float β)
{
    #if DEBUG_AI_SLOW_LEGALITY_CHECK == 1
    std::vector allMoves {getAvailableMoves(maximisingPlayer, tiles, CRs, ePRs, 1, 1)};
    #else
    std::vector allMoves {getAvailableMoves(maximisingPlayer, tiles, CRs, ePRs, 0, 0)};
    #endif // DEBUG_AI_SLOW_LEGALITY_CHECK

    // fprintf(stderr, "DEPTH = %d, PLAYER = %d\n", depth, maximisingPlayer);
    if (!allMoves.size())
    {
        assert(depth != 1);
        if (kingInCheck(tiles, !maximisingPlayer, CRs, ePRs))
            return Move(43, 43, 43, 43);
        return Move(42, 42, 42, 42);
    }

    #if DEBUG_AI_RANDOM == 1
        srand(time(0));
        return allMoves[floor(static_cast<float>(rand()) / RAND_MAX * allMoves.size())];
    #endif // DEBUG_AI_RANDOM

    Move best {allMoves[0]};
    float bestRating = -99999.0f * (maximisingPlayer * 2 - 1);
    for (auto move : allMoves)
    {
        std::vector<std::vector<Tile>> newPosition {tiles};
        CastlingRights newCRs {CRs};
        std::vector<bool> newEPRs {ePRs};

        makeMove(move, newPosition, newCRs, newEPRs);

        int d = depth + 1;
        bool p = !maximisingPlayer;
        float rating;
        bool lastWasCapture {false};

        while (d <= MAX_DEPTH)
        {
            if (d > MIN_DEPTH)
                if (!lastWasCapture)
                    break;

            Move bestResponse = bestMove(newPosition, d, p, newCRs, newEPRs, α, β);

            if (bestResponse.pos1X == 42)
            {
                rating = 0.0f;
                goto compare;
            }

            if (bestResponse.pos1X == 43)
            {
                rating = -9000.0f * (p * 2 - 1);
                goto compare;
            }

            if (moveIsCapture(newPosition, bestResponse))
                lastWasCapture = true;
            else
                lastWasCapture = false;
            makeMove(bestResponse, newPosition, newCRs, newEPRs);
            d++;
            p = !p;
        }

        rating = positionRating(newPosition);
        compare:
        #if DEBUG_USE_ALPHA_BETA_PRUNING == 1
        if (β <= α)
        {
            return move;
        }
        #endif // DEBUG_USE_ALPHA_BETA_PRUNING
        if (maximisingPlayer == 1)
        {
            if (rating > bestRating)
            {
                best = move;
                bestRating = rating;
                α = std::max(bestRating, α);
            }
        }

        else
        {
            if (rating < bestRating)
            {
                best = move;
                bestRating = rating;
                β = std::min(bestRating, β);
            }
        }

    #ifdef DEBUG
    if (depth == 1)
        fprintf(stderr, "Possible move %c%d-%c%d for %s   Expected rating change = %f (%f)\n",
                move.pos1X + 96, move.pos1Y, move.pos2X + 96, move.pos2Y,
                (maximisingPlayer ? "white" : "black"), rating - positionRating(tiles), rating);
    #endif // DEBUG
    }

    return best;
}
