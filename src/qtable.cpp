#include "qtable.hpp"

QTable::
QTable() {
    // std::cout << "QTable::" << __func__ << " ENTER\n";
    std::size_t worldSize = WorldConfig::instance()->worldSize;
    std::size_t numPickup = WorldConfig::instance()->pickupPositions.size();
    std::size_t numDropoff = WorldConfig::instance()->dropoffPositions.size();
    std::size_t maxExp = numDropoff + numPickup + 1;

    table.resize(worldSize * worldSize * (1<<maxExp));
    for(auto &row : table) {
        row.resize(6);
    }

    // bases which will determine what to multiply/divide when encoding/decoding states
    bases.resize(maxExp+2);
    bases[0] = worldSize * 1<<maxExp;
    for(std::size_t i = 1; i < maxExp+2; ++i)
        bases[i] = 1<<(maxExp - i + 1);
}

Operator
QTable::
nextOp(RLState state, std::vector<Operator> applicableOps) {
    // std::cout << "QTable::" << __func__ << " ENTER\n";
    // prioritize picking up or dropping off
    for(auto it = applicableOps.begin(); it != applicableOps.end(); ++it)
        if(*it == Operator::P || *it == Operator::D) return *it;    

    // get column indices of Q-table with highest Q-values for the given state and applicable operators
    double maxQ = table[encodeState(state)][(int)applicableOps[0]];
    for(auto it = applicableOps.begin(); it != applicableOps.end(); ++it) {
        double q = table[encodeState(state)][(int)*it];
        if(q > maxQ)
            maxQ = q;
    }

    std::vector<Operator> maxValOps;
    std::vector<Operator> lowValOps;
    for(auto it = applicableOps.begin(); it != applicableOps.end(); ++it) {
        if(table[encodeState(state)][(int)*it] == maxQ)
            maxValOps.push_back(*it);
        else 
            lowValOps.push_back(*it);
    }

    Policy policy = WorldConfig::instance()->policy;
    if(policy == Policy::PRANDOM) {
        // select applicable operator randomly
        return applicableOps[rand() % applicableOps.size()];
    }
    else 
    if(policy == Policy::PEXPLOIT) {
        // select applicable operator with highest Q-value 80% of the time
        if(rand() % 100 < 80) {
            return maxValOps[rand() % maxValOps.size()];
        }
        else {
            // select applicable operator randomly from operators without the highest Q-value or randomly if no operators remain
            if(lowValOps.size() == 0)
                return maxValOps[rand() % maxValOps.size()];
            else
                return lowValOps[rand() % lowValOps.size()];
        }
    }
    else
    if(policy == Policy::PGREEDY) {
        // select applicable operator with highest Q-value
        return maxValOps[rand() % maxValOps.size()];
    }
    else {
        throw "invalid policy";
    }

}

void
QTable::
update(RLState prevState, RLState currState, Operator op, std::vector<Operator> applicableOps) {
    // std::cout << "QTable::" << __func__ << " ENTER\n";
    if(WorldConfig::instance()->method == Method::SARSA) {
        table[encodeState(prevState)][(int)op] = (
            table[encodeState(prevState)][(int)op] +
            WorldConfig::instance()->alpha * (
                (op == Operator::P || op == Operator::D ? 
                WorldConfig::instance()->reward : WorldConfig::instance()->penalty) + 
                WorldConfig::instance()->gamma * table[encodeState(currState)][(int)nextOp(currState, applicableOps)] -
                table[encodeState(prevState)][(int)op]
            )
        );
    }
    else
    if(WorldConfig::instance()->method == Method::QL) {
        double maxQ = table[encodeState(currState)][(int)applicableOps[0]];
        for(auto it = applicableOps.begin(); it != applicableOps.end(); ++it) {
            double q = table[encodeState(currState)][(int)*it];
            if(q > maxQ)
                maxQ = q;
        }
        table[encodeState(prevState)][(int)op] = (
            (1 - WorldConfig::instance()->alpha) * table[encodeState(prevState)][(int)op] +
            WorldConfig::instance()->alpha * (
                (op == Operator::P || op == Operator::D ? 
                WorldConfig::instance()->reward : WorldConfig::instance()->penalty) +
                WorldConfig::instance()->gamma * maxQ
            )
        );
    }
}

void
QTable::
reset() {
    for(auto i = table.begin(); i != table.end(); ++i)
        for(auto j = i->begin(); j != i->end(); ++j)
            *j = 0.0;
}

std::vector<double>
QTable::
QValues(RLState state) {
    return table[encodeState(state)];
}

std::size_t
QTable::
encodeState(RLState state) {
    // std::cout << "QTable::" << __func__ << " ENTER\n";
    std::size_t index = 0;
    index += state.agentX * bases[0];
    index += state.agentY * bases[1];
    index += state.carryingBlock * bases[2];

    for(std::size_t i = 0; i < state.pickupStates.size(); ++i) {
        index += state.pickupStates[i] * bases[i+3];
    }
    for(std::size_t i = 0; i < state.dropoffStates.size(); ++i) {
        index += state.dropoffStates[i] * bases[i+3+state.pickupStates.size()];
    }
    return index;
}

RLState
QTable::
decodeIndex(std::size_t index) {  
    // std::cout << "QTable::" << __func__ << "\n";
    std::size_t remainder = index;
    std::size_t agentX = remainder / bases[0];
    remainder %= bases[0];
    std::size_t agentY = remainder / bases[1];
    remainder %= bases[1];
    bool carryingBlock = remainder / bases[2];
    remainder %= bases[2];

    std::vector<bool> pickupStates(WorldConfig::instance()->pickupPositions.size());
    for(std::size_t i = 0; i < WorldConfig::instance()->pickupPositions.size(); ++i) {
        pickupStates[i] = (bool)(remainder / bases[i+3]);
        remainder %= bases[i+3];
    }

    std::vector<bool> dropoffStates(WorldConfig::instance()->dropoffPositions.size());
    for(std::size_t i = 0; i < WorldConfig::instance()->dropoffPositions.size(); ++i) {
        dropoffStates[i] = (bool)(remainder / bases[i+3+WorldConfig::instance()->pickupPositions.size()]);
        remainder %= bases[i+3+WorldConfig::instance()->pickupPositions.size()];
    }

    return RLState(agentX, agentY, carryingBlock, pickupStates, dropoffStates);
}

void
QTable::
print() {
    std::cout << "[\n";
    for(auto i = table.begin(); i != table.end(); ++i) {
        std::cout << " [";
        for(auto j = i->begin(); j != i->end(); ++j) {
            std::cout << *j << " ";
        }
        std::cout << "],\n";
    }
    std::cout << "]\n\n";
}
