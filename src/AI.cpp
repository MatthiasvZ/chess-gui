#include "AI.h"
#include "Evaluation.h"
#include "Move.h"
#include "PGN.h"
#include "Common.h"
#include <cmath>
#include <iostream>
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

    if (allMoves.empty())
        return Move(Position(0, 0), Position(0, 0), true);

    Move best = allMoves[0];
    float bestRating = 999'999.0f * (board.toMove * 2 - 1);

    #pragma omp parallel for
    for (auto move : allMoves)
    {
        Board newBoard(board);
        quickMove(move, newBoard);
        newBoard.toMove = !newBoard.toMove;

        int d = depth;
        while (d < AI_MAX_DEPTH)
        {
            auto response = bestMove(newBoard, ++d);
            if (response.empty)
                break;
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

        #if DEBUG_AI_DISPLAY_MOVES == true
        if (depth == 1)
            fprintf(stderr, "Possible move %c%d-%c%d for %s   Expected rating change = %f (%f)\n",
                    move.pos1.x() + 'a', move.pos1.y(), move.pos2.x() + 'a', move.pos2.y(),
                    (board.toMove ? "black" : "white"), rating - evaluate(board), rating);
        #endif // DEBUG_AI_DISPLAY_MOVES
    }
    return best;
}

AI::AI(Board& board, Renderer& renderer)
    : board(board), renderer(renderer)
{

}

void AI::computersTurn()
{
    #if DEBUG_AI_DISPLAY_TIME == true
        auto t_start = std::chrono::high_resolution_clock::now();
    #endif // DEBUG_AI_DISPLAY_TIME

    Move move = bestMove(board);

    #if DEBUG_AI_DISPLAY_TIME == true
        std::cout << "Time taken to calculate response = " << std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - t_start).count()*1000 << std::endl;
    #endif // DEBUG_AI_DISPLAY_TIME

    docMove(move, board);
    makeMove(move, board);

    if (kingInCheck(board))
        renderer.setCheck();
    else
        renderer.clearCheck();
    renderer.setLastMove(move);
}
