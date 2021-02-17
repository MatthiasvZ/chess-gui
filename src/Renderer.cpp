#include "Renderer.h"

std::vector<std::vector<std::vector<float>>> genSquareVertices()
{
    std::vector<std::vector<std::vector<float>>> result;
    result.reserve(8);

    for (int x {0}; x < 8; ++x)
    {
        result.push_back(std::vector<std::vector<float>>());
        result[x].reserve(8);

        for (int y {0}; y < 8; ++y)
        {
            result[x].push_back(std::vector<float>());
            result[x][y].reserve(16);

            result[x][y].push_back(-1.0f + x * 0.25f);
            result[x][y].push_back(-1.0f + y * 0.25f);
            result[x][y].push_back(0.0f);
            result[x][y].push_back(0.0f);

            result[x][y].push_back(-1.0f  + x * 0.25f);
            result[x][y].push_back(-0.75f + y * 0.25f);
            result[x][y].push_back(0.0f);
            result[x][y].push_back(1.0f);

            result[x][y].push_back(-0.75f + x * 0.25f);
            result[x][y].push_back(-0.75f + y * 0.25f);
            result[x][y].push_back(1.0f);
            result[x][y].push_back(1.0f);

            result[x][y].push_back(-0.75f + x * 0.25f);
            result[x][y].push_back(-1.0f  + y * 0.25f);
            result[x][y].push_back(1.0f);
            result[x][y].push_back(0.0f);
        }
    }

    return result;
}

Renderer::Renderer()
    : chequerboard("assets/chequerboard.bmp", 0),
      pawn_W("assets/pawn_white.png", 1),
      pawn_B("assets/pawn_black.png", 2),
      knight_W("assets/knight_white.png", 3),
      knight_B("assets/knight_black.png", 4),
      bishop_W("assets/bishop_white.png", 5),
      bishop_B("assets/bishop_black.png", 6),
      rook_W("assets/rook_white.png", 7),
      rook_B("assets/rook_black.png", 8),
      queen_W("assets/queen_white.png", 9),
      queen_B("assets/queen_black.png", 10),
      king_W("assets/king_white.png", 11),
      king_B("assets/king_black.png", 12),
      selection("assets/selection.png", 13),
      preview("assets/preview.png", 14),
      check("assets/check.png", 15),
      previous("assets/previous.png", 16),

      shader(PT_SHADER_XYUV),

      boardVertices(PT::tVertsSquareXYUV(0.0f, 0.0f, 2.0f, true)),
      indices(PT::tIndsSquares<unsigned char>(1)),
      boardVbo(boardVertices),
      ibo(indices),

      squareVertices(genSquareVertices()),

      selectedPiece(0, 0),
      noSelection(true),
      inCheck(false),
      lastMove(Position(0, 0), Position(0, 0)),
      noLastMove(true)
{
    boardVao.addBuffer(boardVbo, shader.getLayout());

    squareVaos.reserve(8);
    squareVbos.reserve(8);

    for (int x {0}; x < 8; ++x)
    {
        squareVaos.push_back(std::vector<PT::VertexArray>());
        squareVbos.push_back(std::vector<PT::VertexBuffer>());
        squareVaos[x].reserve(8);
        squareVbos[x].reserve(8);

        for (int y {0}; y < 8; ++y)
        {
            squareVaos[x].push_back(PT::VertexArray());
            squareVbos[x].push_back(PT::VertexBuffer(squareVertices[x][y]));
            squareVaos[x][y].addBuffer(squareVbos[x][y], shader.getLayout());
        }
    }

    glDisable(GL_DEPTH_TEST);
}

void Renderer::drawBoard(const Board& board)
{
    PT::clearScreen();

    shader.setUniform1i("texSlot", 0);
    PT::drawVA(boardVao, ibo, shader);

    shader.setUniform1i("texSlot", 14);
    for (auto move : previewMoves)
    {
        PT::drawVA(squareVaos[move.pos2.x()][move.pos2.y()], ibo, shader);
    }

    if (!noLastMove)
    {
        shader.setUniform1i("texSlot", 16);
        PT::drawVA(squareVaos[lastMove.pos1.x()][lastMove.pos1.y()], ibo, shader);
        PT::drawVA(squareVaos[lastMove.pos2.x()][lastMove.pos2.y()], ibo, shader);
    }

    if (!noSelection)
    {
        shader.setUniform1i("texSlot", 13);
        PT::drawVA(squareVaos[selectedPiece.x()][selectedPiece.y()], ibo, shader);
    }

    for (int x {0}; x < 8; ++x)
    {
        for (int y {0}; y < 8; ++y)
        {
            Piece piece = board.at(Position(x, y));

            if (piece.isEmpty())
                continue;

            int texSlot = 0;
            texSlot += piece.color();
            texSlot += 1 * piece.isPawn();
            texSlot += 3 * piece.isKnight();
            texSlot += 5 * piece.isBishop();
            texSlot += 7 * piece.isRook();
            texSlot += 9 * piece.isQueen();
            texSlot += 11 * piece.isKing();


            shader.setUniform1i("texSlot", texSlot);
            PT::drawVA(squareVaos[x][y], ibo, shader);

            if (inCheck)
                if (piece.isKing() && piece.color() == board.toMove)
                {
                    shader.setUniform1i("texSlot", 15);
                    PT::drawVA(squareVaos[x][y], ibo, shader);
                }
        }
    }

    PT::getGlErrors();
}
