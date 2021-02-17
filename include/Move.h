#ifndef MOVE_H
#define MOVE_H

#include <vector>
#include "Board.h"
#include "Position.h"

struct Move
{
    Position pos1;
    Position pos2;
};

bool moveIsLegal(Move move, const Board& board);
std::vector<Move> allLegalMoves(const Board& board);
bool kingInCheck(Board board);
std::vector<Move> slowLegalMoves(const Board& board);
bool checkForStalemate(const Board& board);
bool checkForCheckmate(const Board& board);
void quickMove(Move move, Board& board);
void makeMove(Move move, Board& board);

#endif // MOVE_H
