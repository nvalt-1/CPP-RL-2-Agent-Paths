#ifndef WORLD
#define WORLD

#include "position.hpp"
#include "worldconfig.hpp"
#include "rlstate.hpp"
#include "agent.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

class World {
public:
    static World* instance();

    std::vector<Agent> agents;
    int stepsRun;
    int timesCompleted;
    std::vector<int> pickupStates;
    std::vector<int> dropoffStates;
    
    bool step(bool resetOnFinal = true); // returns true if final state reached in this step
    void resetAll();
    void reset(bool clearTable = false);
    void print();

private:
    static World* singleton;

    bool isFinalState();
    std::vector<Operator> applicableOperators(int agentID, bool ignoreAgents=false);
    void applyOperator(Operator op, int agentID);

    World();
    World(const World&);
    World& operator=(const World&);
    ~World();
};

#endif