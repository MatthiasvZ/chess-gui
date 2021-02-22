#ifndef PGN_H
#define PGN_H

#include "Board.h"
#include "Move.h"
#include <string>

inline std::string pgn;
void docMove(const Move& move, const Board& board);

#endif // PGN_H
