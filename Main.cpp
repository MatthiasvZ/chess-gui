#include "other/FileManagement.h"
#include "gl/Window.h"
#include "gl/Shader.h"
#include "gl/Texture.h"
#include "game/Tile.h"

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

inline bool isTeammate(Piece ownPiece, Piece pieceInQ)
{
    if (pieceInQ == Piece::none)
        return false;
    if ((ownPiece - pieceInQ) % 2 == 0)
        return true;
    return false;
}
bool moveIsLegal(int pos1X, int pos1Y, int pos2X, int pos2Y, std::vector<std::vector<Tile>>& tiles)
{
    bool pathIsClear;
    Piece pieceToMove {tiles.at(pos1X - 1).at(pos1Y - 1).getPiece()};
    Piece pieceToLandOn {tiles.at(pos2X - 1).at(pos2Y - 1).getPiece()};
    static bool whiteCanCastleKs {true};
    static bool whiteCanCastleQs {true};
    static bool blackCanCastleKs {true};
    static bool blackCanCastleQs {true};

    if (pos1X == pos2X && pos1Y == pos2Y)
        return 0;
    if (isTeammate(pieceToMove, pieceToLandOn))
        return 0;

    switch (pieceToMove)
    {
    case Piece::pawn_w:
        if (!window.getPlayer())
            return false;
        if ((pos2X == pos1X + 1 || pos2X == pos1X - 1) &&
            pos2Y == pos1Y + 1 && pieceToLandOn != Piece::none)
            return true;
        if (pos2X != pos1X || tiles.at(pos2X - 1).at(pos2Y - 1).getPiece() != Piece::none)
            return false;
        if (pos2Y == pos1Y + 1)
            return true;
        if (pos1Y == 2 && pos2Y == 4 &&
            tiles.at(pos1X - 1).at(2).getPiece() == Piece::none)
            return true;
        break;
    case Piece::pawn_b:
        if (window.getPlayer())
            return false;
        if ((pos2X == pos1X + 1 || pos2X == pos1X - 1) &&
            pos2Y == pos1Y - 1 && pieceToLandOn != Piece::none)
            return true;
        if (pos2X != pos1X || tiles.at(pos2X - 1).at(pos2Y - 1).getPiece() != Piece::none)
            return false;
        if (pos2Y == pos1Y - 1)
            return true;
        if (pos1Y == 7 && pos2Y == 5 &&
            tiles.at(pos1X - 1).at(5).getPiece() == Piece::none)
            return true;
        break;

    case Piece::knight_w:
        if (!window.getPlayer())
            return false;
        [[fallthrough]];
    case Piece::knight_b:
        if (window.getPlayer() && pieceToMove == knight_b)
            return false;
        if (pos2X == pos1X + 2 && pos2Y == pos1Y + 1)
            return true;
        if (pos2X == pos1X + 2 && pos2Y == pos1Y - 1)
            return true;
        if (pos2X == pos1X - 2 && pos2Y == pos1Y + 1)
            return true;
        if (pos2X == pos1X - 2 && pos2Y == pos1Y - 1)
            return true;
        if (pos2Y == pos1Y + 2 && pos2X == pos1X + 1)
            return true;
        if (pos2Y == pos1Y + 2 && pos2X == pos1X - 1)
            return true;
        if (pos2Y == pos1Y - 2 && pos2X == pos1X + 1)
            return true;
        if (pos2Y == pos1Y - 2 && pos2X == pos1X - 1)
            return true;
        break;

    case Piece::bishop_w:
        if (!window.getPlayer())
            return false;
        [[fallthrough]];
    case Piece::bishop_b:
        if (window.getPlayer() && pieceToMove == Piece::bishop_b)
            return false;
        pathIsClear = true;
        if (pos1X - pos1Y == pos2X - pos2Y)
        {
            for (int ix {pos1X + 1}, iy {pos1Y + 1}; ix < pos2X; ix++, iy++)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
            for (int ix {pos1X - 1}, iy {pos1Y - 1}; ix > pos2X; ix--, iy--)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
        }
        if (pos1X + pos1Y == pos2X + pos2Y)
        {
            for (int ix {pos1X + 1}, iy {pos1Y - 1}; ix < pos2X; ix++, iy--)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
            for (int ix {pos1X - 1}, iy {pos1Y + 1}; ix > pos2X; ix--, iy++)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
        }
        if ((pos1X - pos1Y == pos2X - pos2Y) && pathIsClear) // '/'
            return true;
        if ((pos1X + pos1Y == pos2X + pos2Y) && pathIsClear) // '\'
            return true;
        break;

    case Piece::rook_w:
        if (!window.getPlayer())
            return false;
        [[fallthrough]];
    case Piece::rook_b:
        if (window.getPlayer() && pieceToMove == rook_b)
            return false;
        pathIsClear = true;
        for (int ix {pos1X + 1}; ix < pos2X; ix++)
            if (tiles.at(ix - 1).at(pos1Y - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int ix {pos1X - 1}; ix > pos2X; ix--)
            if (tiles.at(ix - 1).at(pos1Y - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int iy {pos1Y + 1}; iy < pos2Y; iy++)
            if (tiles.at(pos1X - 1).at(iy - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int iy {pos1Y - 1}; iy > pos2Y; iy--)
            if (tiles.at(pos1X - 1).at(iy - 1).getPiece() != Piece::none)
                pathIsClear = false;
        if ((pos1X == pos2X || pos1Y == pos2Y) && pathIsClear)
        {
            if (pos1X == 1 && pos1Y == 1)
                whiteCanCastleQs = false;
            if (pos1X == 8 && pos1Y == 1)
                whiteCanCastleKs = false;
            if (pos1X == 1 && pos1Y == 8)
                blackCanCastleQs = false;
            if (pos1X == 8 && pos1Y == 8)
                blackCanCastleKs = false;
            return true;
        }
        break;

    case Piece::queen_w:
        if (!window.getPlayer())
            return false;
        [[fallthrough]];
    case Piece::queen_b:
        if (window.getPlayer() && pieceToMove == queen_b)
            return false;
        pathIsClear = true;
        if (pos1X == pos2X || pos1Y == pos2Y)
        {
        for (int ix {pos1X + 1}; ix < pos2X; ix++)
            if (tiles.at(ix - 1).at(pos1Y - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int ix {pos1X - 1}; ix > pos2X; ix--)
            if (tiles.at(ix - 1).at(pos1Y - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int iy {pos1Y + 1}; iy < pos2Y; iy++)
            if (tiles.at(pos1X - 1).at(iy - 1).getPiece() != Piece::none)
                pathIsClear = false;
        for (int iy {pos1Y - 1}; iy > pos2Y; iy--)
            if (tiles.at(pos1X - 1).at(iy - 1).getPiece() != Piece::none)
                pathIsClear = false;
        }
        if (pos1X - pos1Y == pos2X - pos2Y)
        {
            for (int ix {pos1X + 1}, iy {pos1Y + 1}; ix < pos2X; ix++, iy++)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
            for (int ix {pos1X - 1}, iy {pos1Y - 1}; ix > pos2X; ix--, iy--)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
        }
        if (pos1X + pos1Y == pos2X + pos2Y)
        {
            for (int ix {pos1X + 1}, iy {pos1Y - 1}; ix < pos2X; ix++, iy--)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
            for (int ix {pos1X - 1}, iy {pos1Y + 1}; ix > pos2X; ix--, iy++)
                if (tiles.at(ix - 1).at(iy - 1).getPiece() != Piece::none)
                    pathIsClear = false;
        }
        if (pos1X - pos1Y == pos2X - pos2Y && pathIsClear)
            return true;
        if (pos1X + pos1Y == pos2X + pos2Y && pathIsClear)
            return true;
        if ((pos1X == pos2X || pos1Y == pos2Y) && pathIsClear)
            return true;
        break;

    case Piece::king_w:
        if (!window.getPlayer())
            return false;
        if (pos2X == 3 && pos2Y == 1 && whiteCanCastleQs && tiles.at(1).at(0).getPiece() == Piece::none &&
            tiles.at(2).at(0).getPiece() == Piece::none &&
            tiles.at(3).at(0).getPiece() == Piece::none &&
            tiles.at(0).at(0).getPiece() == Piece::rook_w)
        {
            tiles.at(0).at(0).setPiece(Piece::none);
            tiles.at(3).at(0).setPiece(Piece::rook_w);
            whiteCanCastleKs = false;
            whiteCanCastleQs = false;
            return true;
        }
        if (pos2X == 7 && pos2Y == 1 && whiteCanCastleKs && tiles.at(5).at(0).getPiece() == Piece::none &&
            tiles.at(6).at(0).getPiece() == Piece::none &&
            tiles.at(7).at(0).getPiece() == Piece::rook_w)
        {
            tiles.at(7).at(0).setPiece(Piece::none);
            tiles.at(5).at(0).setPiece(Piece::rook_w);
            whiteCanCastleKs = false;
            whiteCanCastleQs = false;
            return true;
        }
        [[fallthrough]];
    case Piece::king_b:
        if (window.getPlayer() && pieceToMove == king_b)
            return false;
        if ((pos2X == pos1X + 1 && pos2Y == pos1Y) ||
            (pos2X == pos1X + 1 && pos2Y == pos1Y + 1) ||
            (pos2X == pos1X + 1 && pos2Y == pos1Y - 1) ||
            (pos2X == pos1X - 1 && pos2Y == pos1Y) ||
            (pos2X == pos1X - 1 && pos2Y == pos1Y + 1) ||
            (pos2X == pos1X - 1 && pos2Y == pos1Y - 1) ||
            (pos2Y == pos1Y + 1 && pos2X == pos1X) ||
            (pos2Y == pos1Y - 1 && pos2X == pos1X))
        {
            if (pieceToMove == king_w)
            {
                whiteCanCastleKs = false;
                whiteCanCastleQs = false;
            }
            else
            {
                blackCanCastleKs = false;
                blackCanCastleQs = false;
            }
            return true;
        }
        if (pieceToMove == king_b && pos2X == 3 && pos2Y == 8 && blackCanCastleQs &&
            tiles.at(1).at(7).getPiece() == Piece::none &&
            tiles.at(2).at(7).getPiece() == Piece::none &&
            tiles.at(3).at(7).getPiece() == Piece::none &&
            tiles.at(0).at(7).getPiece() == Piece::rook_b)
        {
            tiles.at(0).at(7).setPiece(Piece::none);
            tiles.at(3).at(7).setPiece(Piece::rook_b);
            blackCanCastleKs = false;
            blackCanCastleQs = false;
            return true;
        }
        if (pieceToMove == king_b && pos2X == 7 && pos2Y == 8 && blackCanCastleKs &&
            tiles.at(5).at(7).getPiece() == Piece::none &&
            tiles.at(6).at(7).getPiece() == Piece::none &&
            tiles.at(7).at(7).getPiece() == Piece::rook_b)
        {
            tiles.at(7).at(7).setPiece(Piece::none);
            tiles.at(5).at(7).setPiece(Piece::rook_b);
            blackCanCastleKs = false;
            blackCanCastleQs = false;
            return true;
        }
    default:
        return false;
        break;
    }

    return false;
}

void updateGame(std::vector<std::vector<Tile>>& tiles)
{
    bool whiteKingFound {false};
    bool blackKingFound {false};

    for (int ix {0}; ix < 8; ix++)
    {
        if (tiles.at(ix).at(0).getPiece() == Piece::pawn_b)
            tiles.at(ix).at(0).setPiece(Piece::queen_b);
        if (tiles.at(ix).at(7).getPiece() == Piece::pawn_w)
            tiles.at(ix).at(7).setPiece(Piece::queen_w);
        for (int iy {0}; iy < 8; iy++)
        {
            if (tiles.at(ix).at(iy).getPiece() == Piece::king_w)
                whiteKingFound = true;
            if (tiles.at(ix).at(iy).getPiece() == Piece::king_b)
                blackKingFound = true;
        }
    }
    if (!whiteKingFound)
    {
        window.setGameOver(1);
        gameOver = true;
    }
    if (!blackKingFound)
    {
        window.setGameOver(0);
        gameOver = true;
    }
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

    int tileX {0};
    int tileY {0};
    int selectedTileX {0};
    int selectedTileY {0};

    while (window.shouldRun())
    {
        renderer.clear();

        shader.setUniform1i("tex", 0);
        renderer.drawVA(boardVao, ibo);

        // Selecting/Moving the pieces
        window.getSelectedTile(tileX, tileY);
        if (!selectedTileX)
        {
            if (!tileX)
                goto postTileSelection;
            if (tiles.at(tileX - 1).at(tileY - 1).getPiece() == Piece::none)
                goto postTileSelection;
            selectedTileX = tileX;
            selectedTileY = tileY;
            window.removeSelection();
        }
        else if (tileX)
        {
            if (moveIsLegal(selectedTileX, selectedTileY, tileX, tileY, tiles))
            {
                tiles.at(tileX - 1).at(tileY - 1).setPiece(
                        tiles.at(selectedTileX - 1).at(selectedTileY - 1).getPiece());
                tiles.at(selectedTileX - 1).at(selectedTileY - 1).setPiece(Piece::none);
                updateGame(tiles);
                if (gameOver)
                    goto gameOver;
                window.setplayer(!window.getPlayer());
                window.updateTitle();
            }
            selectedTileX = 0;
            selectedTileY = 0;
            window.removeSelection();
        }
        postTileSelection: // Skip ifs, if the tile is empty

        // Draw the selection underlay
        if (selectedTileX)
        {
            shader.setUniform1i("tex", 13);
            renderer.drawVA(vaos.at(selectedTileX - 1).at(selectedTileY - 1), ibo);
        }

        // Draw the pieces
        for (int ix {0}; ix < 8; ix++)
            for (int iy {0}; iy < 8; iy++)
            {
                if (!tiles.at(ix).at(iy).getPiece())
                    continue;
                shader.setUniform1i("tex", tiles.at(ix).at(iy).getPiece());
                renderer.drawVA(vaos.at(ix).at(iy), ibo);
            }

        window.countfps();
        window.updateWindow();
        #ifdef DEBUG
            ErrorFeedback::getErrors();
        #endif //DEBUG
    }

    gameOver:
    while (window.shouldRun())
    {
        window.updateWindow();
        #ifdef DEBUG
            ErrorFeedback::getErrors();
        #endif //DEBUG
    }
    return 0;
}
