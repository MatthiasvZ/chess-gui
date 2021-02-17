#ifndef UI_H
#define UI_H

#include "Board.h"
#include "Renderer.h"
#include "Petroleum.h"

class UI
{
    public:
        UI(Board& board, Renderer& renderer);
        void playersTurn(PT::Window& window);
    protected:

    private:
        Board& board;
        Renderer& renderer;

        bool mouseHeld;
        unsigned int tileX;
        unsigned int tileY;
        unsigned int sTileX;
        unsigned int sTileY;
};

#endif // UI_H
