#ifndef COMMON_H
#define COMMON_H

#include "Piece.h"

#define VERSION "0.2.1.1"

#define PLAY_AS WHITE
#define PLAY_AGAINST_AI true
#define AI_VS_AI false

#define RECORD_PGN true

// must be odd for now
#define AI_MAX_DEPTH 3
#define AI_MIN_DEPTH 3

#define DEBUG_AI_RANDOM false
#define DEBUG_SLOW_LEGALITY_CHECK DEBUG_AI_RANDOM

#define DEBUG_AI_DISPLAY_MOVES false
#define DEBUG_AI_DISPLAY_TIME false


#endif // COMMON_H
