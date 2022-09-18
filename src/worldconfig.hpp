#ifndef WORLDCONFIG
#define WORLDCONFIG

#include "position.hpp"
#include <vector>
#include <iostream>

enum class Policy {PRANDOM, PEXPLOIT, PGREEDY};
enum class Method {QL, SARSA};
enum class Operator {N, E, S, W, P, D};

class WorldConfig {
public:
    static WorldConfig* instance();

    // Must be set BEFORE creating World instance
    std::vector<Position> agentStartPositions;
    std::vector<Position> pickupPositions;
    std::vector<Position> dropoffPositions;
    int worldSize;

    // Can be changed any time
    int pickupCapacity;
    int dropoffCapacity;
    double alpha;
    double gamma;
    int penalty;
    int reward;
    Policy policy;
    Method method;

    void print();
    
private:
    static WorldConfig* singleton;

    WorldConfig();
    WorldConfig(const WorldConfig&);
    WorldConfig& operator=(const WorldConfig&);
    ~WorldConfig();
};

#endif