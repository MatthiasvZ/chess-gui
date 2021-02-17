#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Renderer.h"

class AI
{
    public:
        AI(Board& board, Renderer& renderer);
        void computersTurn();

    protected:

    private:
        Board& board;
        Renderer& renderer;
};

#endif // AI_H
