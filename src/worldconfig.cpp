#include "worldconfig.hpp"

WorldConfig* 
WorldConfig::
instance() {
    return (!singleton) ? singleton = new WorldConfig() : singleton;
}

WorldConfig::
WorldConfig() {
    worldSize = 5;
    pickupCapacity = 10;
    dropoffCapacity = 5;

    agentStartPositions = {Position(2,0), Position(2,4)};
    pickupPositions = {Position(4,2), Position(1,3)};
    dropoffPositions = {Position(0,0), Position(4,0), Position(2,2), Position(4,4)};

    policy = Policy::PRANDOM;
    method = Method::QL;

    alpha = 0.3;
    gamma = 0.5;
    penalty = -1;
    reward = 13;
}

void
WorldConfig::
print() {
    std::cout << "WORLD_SIZE       : " << worldSize << "\n";
    std::cout << "PICKUP_CAPACITY  : " << pickupCapacity << "\n";
    std::cout << "DROPOFF_CAPACITY : " << worldSize << "\n";
    std::cout << "POLICY           : " << (policy == Policy::PRANDOM ? "PRANDOM" : policy == Policy::PEXPLOIT ? "PEXPLOIT" : "PGREEDY ") << "\n";
    std::cout << "METHOD           : " << (method == Method::QL ? "QL   " : "SARSA") << "\n";
    std::cout << "ALPHA            : " << alpha << "\n";
    std::cout << "GAMMA            : " << gamma << "\n";
    std::cout << "PENALTY          : " << penalty << "\n";
    std::cout << "REWARD           : " << reward << "\n";

    std::cout << "AGENT_START_POS  : " << "[ ";
    for(auto &pos: agentStartPositions)
        std::cout << "(" << pos.x << ", " << pos.y << ") ";
    std::cout << "]\n";

    std::cout << "PICKUP_POS       : " << "[ ";
    for(auto &pos: pickupPositions)
        std::cout << "(" << pos.x << ", " << pos.y << ") ";
    std::cout << "]\n";

    std::cout << "DROPOFF_POS      : " << "[ ";
    for(auto &pos: dropoffPositions)
        std::cout << "(" << pos.x << ", " << pos.y << ") ";
    std::cout << "]\n\n";
}

WorldConfig::
~WorldConfig() {
    delete singleton;
}
