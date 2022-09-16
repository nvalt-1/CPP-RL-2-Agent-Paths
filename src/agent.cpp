#include "agent.hpp"

Agent::
Agent(int i) {
    // std::cout << "Agent::" << __func__ << " ENTER\n";
    id = i;
    Position startPos = WorldConfig::instance()->agentStartPositions[id];
    pos = Position(startPos.x, startPos.y);
    carryingBlock = false;
    operatorsUsed = 0;
}

Agent::
Agent() {}

RLState 
Agent::
currentState(std::vector<bool> pickupStates, std::vector<bool> dropoffStates) {
    // std::cout << "Agent::" << __func__ << " ENTER\n";
    return RLState(pos.x, pos.y, carryingBlock, pickupStates, dropoffStates);
}

void 
Agent::
reset(bool clearTable) {
    // std::cout << "Agent::" << __func__ << " ENTER\n";
    Position startPos = WorldConfig::instance()->agentStartPositions[id];
    pos.x = startPos.x;
    pos.y = startPos.y;
    carryingBlock = false;
    operatorsUsed = 0;
    if(clearTable) table.reset();
}

Operator
Agent::
nextOp(RLState state, std::vector<Operator> applicableOps) { 
    // std::cout << "Agent::" << __func__ << " ENTER\n";
    return table.nextOp(state, applicableOps);
}

void 
Agent::
updateTable(RLState prevState, RLState currState, Operator op, std::vector<Operator> applicableOps) {  
    // std::cout << "Agent::" << __func__ << " ENTER\n";
    table.update(prevState, currState, op, applicableOps);
}

void 
Agent::
applyOperator(Operator op) {
    // std::cout << "Agent::" << __func__ << " ENTER\n";
    if(op == Operator::N) pos.y += -1;
    if(op == Operator::S) pos.y += 1;
    if(op == Operator::E) pos.x += 1;
    if(op == Operator::W) pos.x += -1;
    if(op == Operator::P) carryingBlock = true;
    if(op == Operator::D) carryingBlock = false;
    operatorsUsed += 1;
}

void
Agent::
print() {
    std::cout << "AGENT_ID         : " << id << "\n";
    std::cout << "AGENT_POS        : " << "(" << pos.x << ", " << pos.y << ")\n";
    std::cout << "CARRYING_BLOCK   : " << carryingBlock << "\n\n";
    // table.print();
}
