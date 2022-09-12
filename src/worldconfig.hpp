#ifndef WORLDCONFIG
#define WORLDCONFIG

#include "position.hpp"
#include <vector>

class WorldConfig {
public:
    static WorldConfig* instance();

    enum class Policy {PRANDOM, PEXPLOIT, PGREEDY};
    enum class Method {QL, SARSA};
    enum class Operator {N, E, S, W, P, D};

    int worldSize;
    int pickupCapacity;
    int dropoffCapacity;

    std::vector<Position> agentStartPositions;
    std::vector<Position> pickupPositions;
    std::vector<Position> dropoffPositions;

    Policy policy;
    Method method;

    double alpha;
    double gamma;
    int penalty;
    int reward;

private:
    static WorldConfig* singleton;
    WorldConfig();
    ~WorldConfig();
};

#endif