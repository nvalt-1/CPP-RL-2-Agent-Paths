#ifndef POSITION
#define POSITION

class Position {
public:
    int x;
    int y;

    bool operator==(const Position &rhs) const;
    Position();
    Position(int x, int y);
};

#endif