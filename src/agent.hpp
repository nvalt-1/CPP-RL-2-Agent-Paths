#ifndef AGENT
#define AGENT

#include "position.hpp"
#include "worldconfig.hpp"
#include "rlstate.hpp"
#include "qtable.hpp"

#include <vector>
#include <iostream>

class Agent {
public:
    int id;
    Position pos;
    bool carryingBlock;

    RLState currentState(std::vector<bool> pickupStates, std::vector<bool> dropoffStates);
    void reset(bool clearTable = false);
    Operator nextOp(RLState state, std::vector<Operator> applicableOps);
    void applyOperator(Operator op);
    void updateTable(RLState prevState, RLState currState, Operator op, std::vector<Operator> applicableOps);
    void print();

    Agent();
    Agent(int id);

private:
    int operatorsUsed;
    QTable table;
};

#endif