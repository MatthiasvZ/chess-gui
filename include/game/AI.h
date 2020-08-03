#ifndef AI_H
#define AI_H

#include "game/Tile.h"
#include "game/Common.h"

float positionRating(const std::vector<std::vector<Tile>>& tiles);
bool positionIsSafe(const std::vector<std::vector<Tile>>& tiles, const std::vector<Move>& allMoves);
Move bestMove(const std::vector<std::vector<Tile>>& tiles, int depth, bool player, const CastlingRights& CRs, const std::vector<bool>& ePRs, float α = -99999.0f, float β = 99999.0f);

#endif // AI_H
