#ifndef AI_H
#define AI_H

#include "game/Tile.h"
#include "game/Common.h"

float positionRating(const std::vector<std::vector<Tile>>& tiles);
Move bestMove(std::vector<std::vector<Tile>> tiles, int depth, bool player, CastlingRights CRsCopy, std::vector<bool> ePRsCopy);

#endif // AI_H
