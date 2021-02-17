#include "Position.h"

Position::Position(unsigned char x, unsigned char y)
    : data(x + (y << 4))
{

}

