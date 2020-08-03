#ifndef MOVE_H
#define MOVE_H

#include "game/Common.h"
#include "game/Tile.h"
#include <vector>

bool kingInCheck(const std::vector<std::vector<Tile>>& tiles, bool whoToMove, const CastlingRights& CRs, const std::vector<bool>& ePRs, unsigned char* posX = nullptr, unsigned char* posY = nullptr);
bool moveIsLegal(Move move, std::vector<std::vector<Tile>>& tiles, bool whoToMove, const CastlingRights& CRs, const std::vector<bool>& ePRs, bool firstRunThrough = 1, bool slowMode = 0);
std::vector<Move> getAvailableMoves(bool whoToMove, std::vector<std::vector<Tile>> tiles, const CastlingRights& CRs, const std::vector<bool>& ePRs, bool firstRunThrough = 1, bool slowMode = 0);
void makeMove(Move move, std::vector<std::vector<Tile>>& tiles, CastlingRights& CRs, std::vector<bool>& ePRs);
void updateGame(std::vector<std::vector<Tile>>& tiles);

#endif // MOVE_H
