#ifndef RLSTATE
#define RLSTATE

#include "worldconfig.hpp"
#include <vector>

class RLState {
public:
    int agentX;
    int agentY;
    bool carryingBlock;
    std::vector<bool> pickupStates;
    std::vector<bool> dropoffStates;

    RLState();
    RLState(int, int, bool, std::vector<bool>, std::vector<bool>);
};

#endif