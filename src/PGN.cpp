#include "PGN.h"
#include "Common.h"

static const std::string playerW = (AI_VS_AI || (PLAY_AGAINST_AI && !(PLAY_AS == WHITE)) ? "[White \"chess-gui " VERSION "\"]\n" : "[White \"?\"]\n");
static const std::string playerB = (AI_VS_AI || (PLAY_AGAINST_AI && !(PLAY_AS == BLACK)) ? "[Black \"chess-gui " VERSION "\"]\n\n" : "[Black \"?\"]\n\n");
static const std::string pgnInfo = playerW + playerB;

void docMove(const Move& move, const Board& board)
{
    #if RECORD_PGN == true
    if (board.toMove == WHITE)
        pgn += (board.fullmoveClock == 1 ? pgnInfo : " ") + std::to_string(board.fullmoveClock) + ".";
    pgn += " ";

    if (board.at(move.pos1).isPawn())
        pgn += "";
    else if (board.at(move.pos1).isKnight())
        pgn += "N";
    else if (board.at(move.pos1).isBishop())
        pgn += "B";
    else if (board.at(move.pos1).isRook())
        pgn += "R";
    else if (board.at(move.pos1).isQueen())
        pgn += "Q";
    else if (board.at(move.pos1).isKing())
        pgn += "K";

    auto allMoves = slowLegalMoves(board);
    bool displayRank = false;
    bool displayFile = false;
    for (auto elt : allMoves)
    {
        if (elt.pos2 == move.pos2 && elt.pos1 != move.pos1 && board.at(elt.pos1).data == board.at(move.pos1).data)
        {
            if (elt.pos1.x() != move.pos1.x())
                displayRank = true;
            else
                displayFile = true;
        }
    }
    if (displayRank)
        pgn += move.pos1.x() + 'a';
    if (displayFile)
        pgn += move.pos1.y() + '1';

    pgn += move.pos2.x() + 'a';
    pgn += move.pos2.y() + '1';
    #endif // RECORD_PGN
}
