#include "qtable.hpp"

QTable::QTable() {
    int worldSize = WorldConfig::instance()->worldSize;
    int numPickup = WorldConfig::instance()->pickupPositions.size();
    int numDropoff = WorldConfig::instance()->dropoffPositions.size();
    int maxExp = numDropoff + numPickup + 1;
    
    table.resize(worldSize * worldSize * (1<<maxExp));
    for(auto &row : table) {
        row.resize(6);
    }

    // bases which will determine what to multiply/divide when encoding/decoding states
    bases.resize(maxExp+2);
    bases[0] = worldSize * 1<<maxExp;
    for(int i = 1; i < maxExp+2; --i)
        bases[i] = 1<<(maxExp - i + 1);
}

WorldConfig::Operator
QTable::nextOp(RLState state, std::vector<WorldConfig::Operator> applicableOps) {
    // prioritize picking up or dropping off
    auto it = std::find(applicableOps.begin(), applicableOps.end(), WorldConfig::Operator::P);
    if(it != applicableOps.end())
        return *it;
    it = std::find(applicableOps.begin(), applicableOps.end(), WorldConfig::Operator::D);
    if(it != applicableOps.end())
        return *it;
    
    // get column indices of Q-table with highest Q-values for the given state and applicable operators
    double maxQ = -1;
    for(auto it = applicableOps.begin(); it != applicableOps.end(); ++it) {
        double q = table[encodeState(state)][(int)*it];
        if(q > maxQ)
            maxQ = q;
    }
    std::vector<WorldConfig::Operator> maxValOps;
    std::copy_if(applicableOps.begin(), applicableOps.end(), maxValOps.begin(), 
        [&](auto &op) {
            return table[encodeState(state)][(int)op] == maxQ;
        });

    WorldConfig::Policy policy = WorldConfig::instance()->policy;
    if(policy == WorldConfig::Policy::PRANDOM) {
        // select applicable operator randomly
        return applicableOps[rand() % applicableOps.size()];
    }
    else 
    if(policy == WorldConfig::Policy::PEXPLOIT) {
        // select applicable operator with highest Q-value 80% of the time
        if(rand() % 100 < 80) {
            return maxValOps[rand() % maxValOps.size()];
        }
        else {
            // select applicable operator randomly from operators without the highest Q-value or randomly if no operators remain
            std::vector<WorldConfig::Operator> setDiff;
            std::set_difference(applicableOps.begin(), applicableOps.end(), maxValOps.begin(), maxValOps.end(), setDiff.begin());

            if(setDiff.size() == 0)
                return applicableOps[rand() % applicableOps.size()];
            else
                return setDiff[rand() % setDiff.size()];
        }
    }
    else
    if(policy == WorldConfig::Policy::PGREEDY) {
        // select applicable operator with highest Q-value
        return maxValOps[rand() % maxValOps.size()];
    }
    else {
        throw "invalid policy";
    }
}

void
QTable::update(RLState prevState, RLState currState, WorldConfig::Operator op, std::vector<WorldConfig::Operator> applicableOps) {
    if(WorldConfig::instance()->method == WorldConfig::Method::SARSA) {
        table[encodeState(prevState)][(int)op] = (
            table[encodeState(prevState)][(int)op] +
            WorldConfig::instance()->alpha * (
                (op == WorldConfig::Operator::P || op == WorldConfig::Operator::D ? 
                WorldConfig::instance()->reward : WorldConfig::instance()->penalty) + 
                WorldConfig::instance()->gamma * table[encodeState(currState)][(int)nextOp(currState, applicableOps)] -
                table[encodeState(prevState)][(int)op]
            )
        );
    }
    else
    if(WorldConfig::instance()->method == WorldConfig::Method::QL) {
        double maxQ = -1;
        for(auto it = applicableOps.begin(); it != applicableOps.end(); ++it) {
            double q = table[encodeState(currState)][(int)*it];
            if(q > maxQ)
                maxQ = q;
        }
        
        table[encodeState(prevState)][(int)op] = (
            (1 - WorldConfig::instance()->alpha) * table[encodeState(prevState)][(int)op] +
            WorldConfig::instance()->alpha * (
                (op == WorldConfig::Operator::P || op == WorldConfig::Operator::D ? 
                WorldConfig::instance()->reward : WorldConfig::instance()->penalty) +
                WorldConfig::instance()->gamma * maxQ
            )
        );
    }
}

void
QTable::reset() {
    for(auto i = table.begin(); i != table.end(); ++i)
        for(auto j = i->begin(); j != i->end(); ++j)
            *j = 0.0;
}

std::vector<double>
QTable::QValues(RLState state) {
    return table[encodeState(state)];
}

int
QTable::encodeState(RLState state) {
    int index = 0;
    index += state.agentX * bases[0];
    index += state.agentY * bases[1];
    index += state.carryingBlock * bases[2];
    for(int i = 0; i < state.pickupStates.size(); ++i)
        index += state.pickupStates[i] * bases[i+3];
    for(int i = 0; i < state.dropoffStates.size(); ++i)
        index += state.dropoffStates[i] * bases[i+3+state.dropoffStates.size()];

    return index;
}

RLState
QTable::decodeIndex(int index) {
    int remainder = index;
    int agentX = remainder / bases[0];
    remainder %= bases[0];
    int agentY = remainder / bases[1];
    remainder %= bases[1];
    bool carryingBlock = remainder / bases[2];
    remainder %= bases[2];

    std::vector<bool> pickupStates(WorldConfig::instance()->pickupPositions.size());
    for(int i = 0; i < WorldConfig::instance()->pickupPositions.size(); ++i) {
        pickupStates[i] = (bool)(remainder / bases[i+3]);
        remainder %= bases[i+3];
    }

    std::vector<bool> dropoffStates(WorldConfig::instance()->dropoffPositions.size());
    for(int i = 0; i < WorldConfig::instance()->dropoffPositions.size(); ++i) {
        dropoffStates[i] = (bool)(remainder / bases[i+3+WorldConfig::instance()->pickupPositions.size()]);
        remainder %= bases[i+3+WorldConfig::instance()->pickupPositions.size()];
    }

    return RLState(agentX, agentY, carryingBlock, pickupStates, dropoffStates);
}
