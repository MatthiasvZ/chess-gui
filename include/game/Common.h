#ifndef COMMON_H
#define COMMON_H

#define PLAY_AS 1 // 1 = white, 0 = black
#define PLAY_AGAINST_AI true
#define AI_VS_AI false
#define AI_ANALYSE false // causes a lot of lag in between moves

#define MIN_DEPTH 4 // must be odd?
#define MAX_DEPTH 4 // must be odd?

#define DEBUG_AI_RANDOM 0 // makes random moves
#define DEBUG_AI_SLOW_LEGALITY_CHECK 0
#define DEBUG_AI_VALUE_MATERIAL_ONLY 0 // don't try to centralise pieces
#define DEBUG_USE_ALPHA_BETA_PRUNING 0
// ONE AND ONLY ONE MUST BE SET TO 1
#define L10N_EN_GB 1
#define L10N_DE_DE 0

struct Move
{
    unsigned char pos1X;
    unsigned char pos1Y;
    unsigned char pos2X;
    unsigned char pos2Y;
};

struct CastlingRights
{
    bool whiteCanCastleKs {true};
    bool whiteCanCastleQs {true};
    bool blackCanCastleKs {true};
    bool blackCanCastleQs {true};
};

enum Piece : int
{
    none,
    pawn_w, pawn_b,
    knight_w, knight_b,
    bishop_w, bishop_b,
    rook_w, rook_b,
    queen_w, queen_b,
    king_w, king_b
};

inline bool isTeammate(Piece ownPiece, Piece pieceInQ)
{
    if (pieceInQ == Piece::none)
        return false;
    if ((ownPiece - pieceInQ) % 2 == 0)
        return true;
    return false;
}

#endif // COMMON_H
