#include "Position.h"

Position::Position(unsigned char x, unsigned char y)
    : data(y + (x << 3))
{

}

