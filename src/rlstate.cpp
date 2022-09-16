#include "rlstate.hpp"

RLState::
RLState() {
    agentX = 0;
    agentY = 0;
    carryingBlock = false;

    int numPickup = WorldConfig::instance()->pickupPositions.size();
    int numDropoff = WorldConfig::instance()->dropoffPositions.size();
    pickupStates.resize(numPickup);
    dropoffStates.resize(numDropoff);
}

RLState::
RLState(int agentX, int agentY, bool carryingBlock, std::vector<bool> pickupStates, std::vector<bool> dropoffStates) : 
    agentX(agentX), agentY(agentY), carryingBlock(carryingBlock), pickupStates(pickupStates), dropoffStates(dropoffStates) {}

void
RLState::
print() {
    std::cout << "STATE_X          : " << agentX << "\n";
    std::cout << "STATE_Y          : " << agentY << "\n";
    std::cout << "STATE_CARRYING   : " << carryingBlock << "\n";

    std::cout << "PICKUP_STATES    : " << "[";
    for(auto state: pickupStates)
        std::cout << state << ", ";
    std::cout << "]\n";

    std::cout << "DROPOFF_STATES   : " << "[";
    for(auto state: dropoffStates)
        std::cout << state << ", ";
    std::cout << "]\n\n";
}