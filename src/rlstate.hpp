#ifndef RLSTATE
#define RLSTATE

#include "worldconfig.hpp"
#include <vector>
#include <iostream>

class RLState {
public:
    int agentX;
    int agentY;
    bool carryingBlock;
    std::vector<bool> pickupStates;
    std::vector<bool> dropoffStates;

    void print();
    RLState();
    RLState(int agentX, int agentY, bool carryingBlock, std::vector<bool> pickupStates, std::vector<bool> dropoffStates);
};

#endif