#include "position.hpp"

Position::
Position() {
    x = 0;
    y = 0;
}

Position::
Position(int x, int y) : x(x), y(y) {}

bool 
Position::
operator==(const Position &rhs) const {
    return x == rhs.x && y == rhs.y;
}
