#include "Board.h"
#include "Evaluation.h"
#include "Renderer.h"
#include "AI.h"
#include "UI.h"
#include "Common.h"
#include "Move.h"
#include "PGN.h"
#include <iostream>
#include <chrono>

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

    auto t_start = std::chrono::high_resolution_clock::now();
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
    std::cout << "Total time = " << std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - t_start).count()*1000 << std::endl;
    #if RECORD_PGN == true
        std::cout << pgn << std::endl;
    #endif // RECORD_PGN

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
