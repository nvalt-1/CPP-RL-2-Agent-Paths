#include "rlstate.hpp"

RLState::RLState() {
    agentX = 0;
    agentY = 0;
    carryingBlock = false;

    int numPickup = WorldConfig::instance()->pickupPositions.size();
    int numDropoff = WorldConfig::instance()->dropoffPositions.size();
    pickupStates.resize(numPickup);
    dropoffStates.resize(numDropoff);
}

RLState::RLState(int agentX, int agentY, bool carryingBlock, std::vector<bool> pickupStates, std::vector<bool> dropoffStates) : 
    agentX(agentX), agentY(agentY), carryingBlock(carryingBlock), pickupStates(pickupStates), dropoffStates(dropoffStates) {}
