#include "game/Tile.h"

Tile::Tile(unsigned char posX, unsigned char posY)
{
    if (posY == 2)
        piece = Piece::pawn_w;
    else if (posY == 7)
        piece = Piece::pawn_b;

    else if (posY == 1)
        switch (posX)
        {
        case 1:
            piece = Piece::rook_w;
            break;
        case 2:
            piece = Piece::knight_w;
            break;
        case 3:
            piece = Piece::bishop_w;
            break;
        case 4:
            piece = Piece::queen_w;
            break;
        case 5:
            piece = Piece::king_w;
            break;
        case 6:
            piece = bishop_w;
            break;
        case 7:
            piece = knight_w;
            break;
        case 8:
            piece = rook_w;
            break;

        default:
            break;
        }

        else if (posY == 8)
        switch (posX)
        {
        case 1:
            piece = Piece::rook_b;
            break;
        case 2:
            piece = Piece::knight_b;
            break;
        case 3:
            piece = Piece::bishop_b;
            break;
        case 4:
            piece = Piece::queen_b;
            break;
        case 5:
            piece = Piece::king_b;
            break;
        case 6:
            piece = bishop_b;
            break;
        case 7:
            piece = knight_b;
            break;
        case 8:
            piece = rook_b;
            break;

        default:
            break;
        }

        else piece = Piece::none;

    vertices.reserve(16);
    vertices.push_back(posX * 1.0f/4 - 1.0f/4 - 1); // Top-left X
    vertices.push_back(posY * 1.0f/4 - 1);          // Top-left Y
    vertices.push_back(0.0f);                       // Top-left U
    vertices.push_back(1.0f);                       // Top-left V

    vertices.push_back(posX * 1.0f/4 - 1);          // Top-right X
    vertices.push_back(posY * 1.0f/4 - 1);          // Top-right Y
    vertices.push_back(1.0f);                       // Top-right U
    vertices.push_back(1.0f);                       // Top-right V

    vertices.push_back(posX * 1.0f/4 - 1);          // Bottom-right X
    vertices.push_back(posY * 1.0f/4 - 1.0f/4 - 1); // Bottom-right Y
    vertices.push_back(1.0f);                       // Bottom-right U
    vertices.push_back(0.0f);                       // Bottom-right V

    vertices.push_back(posX * 1.0f/4 - 1.0f/4 - 1); // Bottom-left X
    vertices.push_back(posY * 1.0f/4 - 1.0f/4 - 1); // Bottom-left Y
    vertices.push_back(0.0f);                       // Bottom-left U
    vertices.push_back(0.0f);                       // Bottom-left V
}

Tile::~Tile()
{
    //dtor
}
