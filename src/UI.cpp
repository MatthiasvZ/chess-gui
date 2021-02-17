#include "UI.h"

#include "Move.h"


UI::UI(Board& board, Renderer& renderer)
    : board(board), renderer(renderer), mouseHeld(false), tileX(0), tileY(0), sTileX(0), sTileY(0)
{

}

void UI::playersTurn(PT::Window& window)
{
    if (window.getInputs()->mouse1 && !mouseHeld)
    {
        double posX, posY;
        int wWidth, wHeight;
        glfwGetWindowSize(window.getGLFWWindow(), &wWidth, &wHeight);
        window.getCursorPos(&posX, &posY);
        tileX = ceil((8*posX) / wWidth);
        tileY = ceil(8 - (8*posY) / wHeight);

        if (!sTileX && (board.at(Position(tileX - 1, tileY - 1)) == NONE ||
            board.at(Position(tileX - 1, tileY - 1)).color() != board.toMove))
        {
            tileX = 0;
            tileY = 0;
        }

        mouseHeld = true;
    }
    else if (!window.getInputs()->mouse1)
        mouseHeld = false;

    if (!sTileX && tileX)
    {
        sTileX = tileX;
        sTileY = tileY;
        tileX = 0;
        tileY = 0;

        renderer.setSelection(Position(sTileX - 1, sTileY - 1));

        std::vector<Move> previewMoves;
        for (auto move : allLegalMoves(board))
            if (move.pos1 == Position(sTileX - 1, sTileY - 1))
            {
                Board tempBoard(board);
                makeMove(move, tempBoard);
                tempBoard.toMove = !tempBoard.toMove;

                if (!kingInCheck(tempBoard))
                    previewMoves.push_back(move);
            }
        renderer.setPreviewMoves(previewMoves);
    }

    if (sTileX && tileX)
    {
        if (moveIsLegal(Move(Position(sTileX - 1, sTileY - 1), Position(tileX - 1, tileY - 1)), board))
        {
            Board tempBoard(board);
            makeMove(Move(Position(sTileX - 1, sTileY - 1), Position(tileX - 1, tileY - 1)), tempBoard);
            tempBoard.toMove = !tempBoard.toMove;
            if (kingInCheck(tempBoard))
                goto outOfIf;

            makeMove(Move(Position(sTileX - 1, sTileY - 1), Position(tileX - 1, tileY - 1)), board);

            if (kingInCheck(board))
                renderer.setCheck();
            else
                renderer.clearCheck();
            renderer.setLastMove(Move(Position(sTileX - 1, sTileY - 1), Position(tileX - 1, tileY - 1)));
        }
        outOfIf:

        sTileX = 0;
        sTileY = 0;
        tileX = 0;
        tileY = 0;
        renderer.clearSelection();
        renderer.clearPreviewMoves();
    }
}
