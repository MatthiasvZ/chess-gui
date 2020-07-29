#include "other/FileManagement.h"
#include "gl/Window.h"
#include "gl/Shader.h"
#include "gl/Texture.h"
#include "game/Common.h"
#include "game/Tile.h"
#include "game/Move.h"
#include "game/AI.h"

#ifdef DEBUG
#include "other/ErrorFeedback.h"
#endif //DEBUG

#include <iostream>
#include <vector>

namespace
{
Window window;
bool gameOver {false};
}

unsigned char kingInCheckX {0};
unsigned char kingInCheckY {0};

Move lastMove;

void updateGame(std::vector<std::vector<Tile>>& tiles, const CastlingRights& CRs, const std::vector<bool>& ePRs)
{
    if (!kingInCheck(tiles, !window.getPlayer(), CRs, ePRs, &kingInCheckX, &kingInCheckY))
    {
        kingInCheckX = 0;
        kingInCheckY = 0;
    }
    else if (getAvailableMoves(!window.getPlayer(), tiles, CRs, ePRs).size() == 0)
    {
        window.setGameOver(window.getPlayer());
        gameOver = true;
    }
    if (getAvailableMoves(!window.getPlayer(), tiles, CRs, ePRs).size() == 0 && !gameOver)
    {
        window.setGameOver(!window.getPlayer() + 3);
        gameOver = true;
    }

    bool allPawnsGone {true};
    for (int ix {0}; ix < 8; ix++)
        for (int iy {0}; iy < 8; iy++)
            if (tiles.at(ix).at(iy).getPiece() == Piece::pawn_w ||
                tiles.at(ix).at(iy).getPiece() == Piece::pawn_b)
                allPawnsGone = false;

    if (allPawnsGone)
    {
        unsigned char bishopCount_W {0};
        unsigned char bishopCount_B {0};
        unsigned char knightCount_W {0};
        unsigned char knightCount_B {0};
        bool majorPieceFound {false};

        for (int ix {0}; ix < 8; ix++)
            for (int iy {0}; iy < 8; iy++)
            {
                if (tiles.at(ix).at(iy).getPiece() == Piece::bishop_w)
                    bishopCount_W++;
                if (tiles.at(ix).at(iy).getPiece() == Piece::bishop_b)
                    bishopCount_B++;
                if (tiles.at(ix).at(iy).getPiece() == Piece::knight_w)
                    knightCount_W++;
                if (tiles.at(ix).at(iy).getPiece() == Piece::knight_b)
                    knightCount_B++;
                if (tiles.at(ix).at(iy).getPiece() == Piece::rook_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::rook_b ||
                    tiles.at(ix).at(iy).getPiece() == Piece::queen_w ||
                    tiles.at(ix).at(iy).getPiece() == Piece::queen_b)
                    majorPieceFound = true;
            }
        if (majorPieceFound == false &&
            bishopCount_W < 2 && bishopCount_B < 2 &&
            knightCount_W < 3 && knightCount_B < 3 &&
            !(bishopCount_W == 1 && knightCount_W > 0) &&
            !(bishopCount_B == 1 && knightCount_B > 0))
        {
            gameOver = true;
            window.setGameOver(2);
        }
    }
    fprintf(stderr, "Rating = %f\n", positionRating(tiles));
}

int main()
{
    FileManagement::setWorkingDir();
    FileManagement::createFolders();

    SourcePackage srcpkg(Shader::readFromFile("src/shaders/BasicTexVertex.glsl"), \
                         Shader::readFromFile("src/shaders/BasicTexFragment.glsl"));
    Shader shader(srcpkg);
    shader.bindShader();
    //Texture atlas("assets/texAtlas.bmp", 0);
    //shader.setUniform1i("tex", 0);

    Renderer renderer;

    IndexBuffer ibo;
    VertexBufferLayout layout;

    unsigned int indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);

    ibo.addData(indices, sizeof(indices) / sizeof(*indices));

    std::vector<std::vector<VertexArray>> vaos;
    std::vector<std::vector<VertexBuffer>> vbos;
    std::vector<std::vector<Tile>> tiles;

    vaos.reserve(8);
    vbos.reserve(8);
    tiles.reserve(8);
    for (int ix {0}; ix < 8; ix++)
    {
        vaos.push_back(std::vector<VertexArray>());
        vbos.push_back(std::vector<VertexBuffer>());
        tiles.push_back(std::vector<Tile>());
        vaos.at(ix).reserve(8);
        vbos.at(ix).reserve(8);
        tiles.at(ix).reserve(8);

        for (int iy {0}; iy < 8; iy++)
        {
            tiles.at(ix).push_back(Tile(ix + 1, iy + 1));
            vaos.at(ix).push_back(VertexArray());
            vaos.at(ix).at(iy).genArray();
            vbos.at(ix).push_back(VertexBuffer());
            vbos.at(ix).at(iy).addData(tiles.at(ix).at(iy).getVertices().data(), 16 * sizeof(GL_FLOAT));
            vaos.at(ix).at(iy).addBuffer(vbos.at(ix).at(iy), layout);
        }
    }

    float boardVertices[] =
    {// X       Y     U     V
        -1.0f,  1.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f
    };

    VertexArray boardVao;
    VertexBuffer boardVbo;
    boardVao.genArray();
    boardVbo.addData(boardVertices, 16 * sizeof(GL_FLOAT));
    boardVao.addBuffer(boardVbo, layout);

    Texture chequerboard("assets/chequerboard.bmp", 0);
    Texture pawn_W("assets/pawn_white.png", 1);
    Texture pawn_B("assets/pawn_black.png", 2);
    Texture knight_W("assets/knight_white.png", 3);
    Texture knight_B("assets/knight_black.png", 4);
    Texture bishop_W("assets/bishop_white.png", 5);
    Texture bishop_B("assets/bishop_black.png", 6);
    Texture rook_W("assets/rook_white.png", 7);
    Texture rook_B("assets/rook_black.png", 8);
    Texture queen_W("assets/queen_white.png", 9);
    Texture queen_B("assets/queen_black.png", 10);
    Texture king_W("assets/king_white.png", 11);
    Texture king_B("assets/king_black.png", 12);
    Texture selection("assets/selection.png", 13);
    Texture preview("assets/preview.png", 14);
    Texture check("assets/check.png", 15);
    Texture previous("assets/previous.png", 16);

    int tileX {0};
    int tileY {0};
    int selectedTileX {0};
    int selectedTileY {0};
    CastlingRights CRs;
    std::vector<bool> ePRs {false, false, false, false, false, false, false, false};

    // Main loop
    while (window.shouldRun())
    {
        // Selecting/Moving the pieces
        window.getSelectedTile(tileX, tileY);
        if ((window.getPlayer() == PLAY_AS || !PLAY_AGAINST_AI) && !AI_VS_AI)
        {
            if (!selectedTileX)
            {
                if (!tileX)
                    goto draw;
                if (tiles.at(tileX - 1).at(tileY - 1).getPiece() == Piece::none)
                    goto draw;
                selectedTileX = tileX;
                selectedTileY = tileY;
                window.removeSelection();
            }
            else if (tileX)
            {
                Move move(selectedTileX, selectedTileY, tileX, tileY);
                if (moveIsLegal(move, tiles, window.getPlayer(), CRs, ePRs))
                {
                    makeMove(move, tiles, CRs, ePRs);
                    lastMove = move;
                    updateGame(tiles, CRs, ePRs);
                    if (gameOver)
                        goto draw;
                    window.setplayer(!window.getPlayer());
                    window.updateTitle();
                }
                selectedTileX = 0;
                selectedTileY = 0;
                window.removeSelection();
            }
        }
        else // Have the AI play
        {
            Move move = bestMove(tiles, 1, window.getPlayer(), CRs, ePRs);
            makeMove(move, tiles, CRs, ePRs);
            lastMove = move;
            updateGame(tiles, CRs, ePRs);
            if (gameOver)
                goto draw;
            window.setplayer(!window.getPlayer());
            window.updateTitle();
        }

        draw:
        // Draw the pieces
        renderer.clear();

        shader.setUniform1i("tex", 0);
        renderer.drawVA(boardVao, ibo);

        if (selectedTileX)
        {
            shader.setUniform1i("tex", 13);
            renderer.drawVA(vaos.at(selectedTileX - 1).at(selectedTileY - 1), ibo);

            shader.setUniform1i("tex", 14);
            std::vector<Move> allMoves {getAvailableMoves(window.getPlayer(), tiles, CRs, ePRs)};
            std::vector<Move> allEnemyMoves {getAvailableMoves(!window.getPlayer(), tiles, CRs, ePRs)};
            allMoves.insert(allMoves.end(), allEnemyMoves.begin(), allEnemyMoves.end());

            for (auto move : allMoves)
                if (move.pos1X == selectedTileX && move.pos1Y == selectedTileY)
                    renderer.drawVA(vaos.at(move.pos2X - 1).at(move.pos2Y - 1), ibo);
        }
        if (kingInCheckX)
        {
            shader.setUniform1i("tex", 15);
            renderer.drawVA(vaos.at(kingInCheckX - 1).at(kingInCheckY - 1), ibo);
        }

        if (lastMove.pos1X)
        {
            shader.setUniform1i("tex", 16);
            renderer.drawVA(vaos.at(lastMove.pos1X - 1).at(lastMove.pos1Y - 1), ibo);
            renderer.drawVA(vaos.at(lastMove.pos2X - 1).at(lastMove.pos2Y - 1), ibo);
        }

        for (int ix {0}; ix < 8; ix++)
            for (int iy {0}; iy < 8; iy++)
            {
                if (!tiles.at(ix).at(iy).getPiece())
                    continue;
                shader.setUniform1i("tex", tiles.at(ix).at(iy).getPiece());
                renderer.drawVA(vaos.at(ix).at(iy), ibo);
            }

        if (!gameOver)
            window.countfps();
        window.updateWindow();
        #ifdef DEBUG
            ErrorFeedback::getErrors();
        #endif //DEBUG
        if (gameOver)
            break;
    }

    // Game-Over Screen
    while (window.shouldRun())
    {
        renderer.clear();

        shader.setUniform1i("tex", 0);
        renderer.drawVA(boardVao, ibo);

        if (kingInCheckX)
        {
            shader.setUniform1i("tex", 15);
            renderer.drawVA(vaos.at(kingInCheckX - 1).at(kingInCheckY - 1), ibo);
        }

        for (int ix {0}; ix < 8; ix++)
            for (int iy {0}; iy < 8; iy++)
            {
                if (!tiles.at(ix).at(iy).getPiece())
                    continue;
                shader.setUniform1i("tex", tiles.at(ix).at(iy).getPiece());
                renderer.drawVA(vaos.at(ix).at(iy), ibo);
            }
        window.updateWindow();
    }
    return 0;
}
