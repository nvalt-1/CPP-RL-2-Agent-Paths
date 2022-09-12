#include "worldconfig.hpp"

WorldConfig* WorldConfig::instance() {
    if(singleton == nullptr) {
        singleton = new WorldConfig();
    }
    return singleton;
}

WorldConfig::WorldConfig() {
    int worldSize = 5;
    int pickupCapacity = 10;
    int dropoffCapacity = 5;

    std::vector<Position> agentStartPositions{Position(0,2), Position(4,2)};
    std::vector<Position> pickupPositions{Position(2,4), Position(3,1)};
    std::vector<Position> dropoffPositions{Position(0,0), Position(0,4), Position(2,2), Position(4,4)};

    Policy policy = Policy::PRANDOM;
    Method method = Method::QL;

    double alpha = 0.3;
    double gamma = 0.5;
    int penalty = -1;
    int reward = 13;
}

WorldConfig::~WorldConfig() {
    delete singleton;
}
