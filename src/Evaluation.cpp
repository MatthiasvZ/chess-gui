#include "Evaluation.h"

float evaluate(Board board)
{
    switch (board.outcome)
    {
        case WIN_FOR_BLACK:
            return -900'000.0f;
            break;
        case DRAWN:
            return 0.0f;
            break;
        case WIN_FOR_WHITE:
            return 900'000.0f;
            break;
    }

    constexpr float OWN_KING = 950'000.0f;
    constexpr float KING     = 500'000.0f;
    constexpr float QUEEN    = 9.0f;
    constexpr float ROOK     = 5.0f;
    constexpr float BISHOP   = 3.2f;
    constexpr float KNIGHT   = 3.0f;
    constexpr float PAWN     = 0.8f;

    float rating = 0.0f;

    for (int x {0}; x < 8; ++x)
        for (int y {0}; y < 8; ++y)
        {
            switch (board.at(Position(x, y)).data)
            {
                case PAWN_W:
                    rating += PAWN;
                    break;
                case PAWN_B:
                    rating -= PAWN;
                    break;
                case KNIGHT_W:
                    rating += KNIGHT;
                    break;
                case KNIGHT_B:
                    rating -= KNIGHT;
                    break;
                case BISHOP_W:
                    rating += BISHOP;
                    break;
                case BISHOP_B:
                    rating -= BISHOP;
                    break;
                case ROOK_W:
                    rating += ROOK;
                    break;
                case ROOK_B:
                    rating -= ROOK;
                    break;
                case QUEEN_W:
                    rating += QUEEN;
                    break;
                case QUEEN_B:
                    rating -= QUEEN;
                    break;
                case KING_W:
                    if (board.toMove == WHITE)
                        rating += OWN_KING;
                    else
                        rating += KING;
                    break;
                case KING_B:
                    if (board.toMove == BLACK)
                        rating -= OWN_KING;
                    else
                        rating -= KING;
                    break;
            }
        }
    if (board.toMove == WHITE)
    {
        rating -= OWN_KING;
        rating += KING;
    }
    else
    {
        rating += OWN_KING;
        rating -= KING;
    }

    return rating;
}
