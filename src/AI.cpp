#include "AI.h"
#include "Evaluation.h"
#include "Move.h"
#include "Common.h"
#include <cmath>
#include <chrono>

Move bestMove(const Board& board, int depth = 1, float α = -999'999.0f, float β = 999'999.0f)
{
    #if DEBUG_SLOW_LEGALITY_CHECK == true
        std::vector<Move> allMoves(slowLegalMoves(board));
    #else
        std::vector<Move> allMoves(allLegalMoves(board));
    #endif // DEBUG_SLOW_LEGALITY_CHECK
    #if DEBUG_AI_RANDOM == true
        srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count());
        return allMoves[floor(static_cast<float>(rand()) / RAND_MAX * allMoves.size())];
    #endif // DEBUG_AI_RANDOM

    Move best = allMoves[0];
    float bestRating = 999'999.0f * (board.toMove * 2 - 1);
    for (auto move : allMoves)
    {
        Board newBoard(board);
        quickMove(move, newBoard);
        newBoard.toMove = !newBoard.toMove;

        int d = depth;
        while (d < 4) {
            quickMove(bestMove(newBoard, ++d), newBoard);
            newBoard.toMove = !newBoard.toMove;
        }
        float rating = evaluate(newBoard);

        if (board.toMove == WHITE)
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
                    move.pos1.x() + 97, move.pos1.y(), move.pos2.x() + 97, move.pos2.y(),
                    (board.toMove ? "black" : "white"), rating - evaluate(board), rating);
        #endif // DEBUG
    }
    return best;
}

AI::AI(Board& board, Renderer& renderer)
    : board(board), renderer(renderer)
{

}

void AI::computersTurn()
{
    Move move = bestMove(board);
    makeMove(move, board);

    if (kingInCheck(board))
        renderer.setCheck();
    else
        renderer.clearCheck();
    renderer.setLastMove(move);
}
