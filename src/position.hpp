#ifndef POSITION
#define POSITION

#include <functional>

class Position {
public:
    int x;
    int y;

    bool operator==(const Position& rhs) const;
    Position();
    Position(int x, int y);
};

class PositionHash {
public:
    std::size_t operator()(const Position& p) const {
        return (std::hash<int>{}(p.x)) ^ (std::hash<int>{}(p.y));
    }
};

#endif