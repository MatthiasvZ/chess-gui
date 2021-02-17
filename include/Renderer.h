#ifndef RENDERER_H
#define RENDERER_H

#include "Petroleum.h"
#include "Board.h"
#include "Move.h"

class Renderer
{
    public:
        Renderer();
        void drawBoard(const Board& board);
        inline void setCheck() { inCheck = true; }
        inline void clearCheck() { inCheck = false; }
        void setSelection(Position pos) { selectedPiece = pos; noSelection = false; }
        void clearSelection() { noSelection = true; }
        void setPreviewMoves(std::vector<Move> pMs) { previewMoves = pMs; }
        void clearPreviewMoves() { previewMoves.clear(); }
        void setLastMove(Move move) { lastMove = move; noLastMove = false; }

    protected:

    private:
        PT::Texture chequerboard;
        PT::Texture pawn_W;
        PT::Texture pawn_B;
        PT::Texture knight_W;
        PT::Texture knight_B;
        PT::Texture bishop_W;
        PT::Texture bishop_B;
        PT::Texture rook_W;
        PT::Texture rook_B;
        PT::Texture queen_W;
        PT::Texture queen_B;
        PT::Texture king_W;
        PT::Texture king_B;
        PT::Texture selection;
        PT::Texture preview;
        PT::Texture check;
        PT::Texture previous;

        PT::Shader shader;
        std::vector<float> boardVertices;
        std::vector<unsigned char> indices;
        PT::VertexArray boardVao;
        PT::VertexBuffer boardVbo;
        PT::IndexBuffer ibo;

        std::vector<std::vector<std::vector<float>>> squareVertices;
        std::vector<std::vector<PT::VertexArray>> squareVaos;
        std::vector<std::vector<PT::VertexBuffer>> squareVbos;

        Position selectedPiece;
        bool noSelection;
        bool inCheck;
        std::vector<Move> previewMoves;
        Move lastMove;
        bool noLastMove;
};

#endif // RENDERER_H
