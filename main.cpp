#include "Board.h"
#include "Evaluation.h"
#include "Renderer.h"
#include "AI.h"
#include "UI.h"
#include "Common.h"
#include "Move.h"
#include <iostream>

int main()
{
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    PT::Config cfg;
    cfg.msaa = 0;
    cfg.vsync = 1;

    PT::Window window(cfg);
    PT::initGL(cfg);
    window.changeTitle("Chess");

    Renderer renderer;
    if (kingInCheck(board))
        renderer.setCheck();

    UI ui(board, renderer);
    AI ai(board, renderer);

    while (window.shouldRun() && board.outcome == ONGOING)
    {
        renderer.drawBoard(board);

        if ((board.toMove == PLAY_AS || !PLAY_AGAINST_AI) && !AI_VS_AI)
            ui.playersTurn(window);
        else
            ai.computersTurn();

        // std::cerr << "Eval: " << evaluate(board) << std::endl;
        window.update();
        PT::doEvents();
    }

    switch (board.outcome)
    {
        case WIN_FOR_BLACK:
            window.changeTitle("Black has won!");
            break;
        case DRAWN:
            window.changeTitle("It's a draw!");
            break;
        case WIN_FOR_WHITE:
            window.changeTitle("White has won!");
            break;
    }

    while (window.shouldRun())
    {
        renderer.drawBoard(board);
        window.update();
        PT::doEvents();
    }

    return 0;
}
