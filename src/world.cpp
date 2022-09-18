#include "world.hpp"

World* 
World::
instance() {
    return (!singleton) ? singleton = new World() : singleton;
}

World::
World() {
    // std::cout << "World::" << __func__ << " ENTER\n";
    int numAgents = WorldConfig::instance()->agentStartPositions.size();
    agents.resize(numAgents);
    for(int i = 0; i < numAgents; ++i)
        agents[i] = Agent(i);

    stepsRun = 0;
    timesCompleted = 0;

    int numPickup = WorldConfig::instance()->pickupPositions.size();
    pickupStates.resize(numPickup);

    for(int i = 0; i < numPickup; ++i)
        pickupStates[i] = WorldConfig::instance()->pickupCapacity;

    int numDropoff = WorldConfig::instance()->dropoffPositions.size();
    dropoffStates.resize(numDropoff);

    for(int i = 0; i < numDropoff; ++i)
        dropoffStates[i] = 0;
}

bool
World::
step(bool resetOnFinal) {  
    // std::cout << "World::" << __func__ << " ENTER\n";
    stepsRun += 1;
    for(auto it = agents.begin(); it != agents.end(); ++it) {
        std::vector<bool> pickupBool;
        int numPickup = WorldConfig::instance()->pickupPositions.size();
        pickupBool.resize(numPickup);
        for(int i = 0; i < numPickup; ++i)
            pickupBool[i] = pickupStates[i] != 0;
        
        std::vector<bool> dropoffBool;
        int numDropoff = WorldConfig::instance()->dropoffPositions.size();
        dropoffBool.resize(numDropoff);
        for(int i = 0; i < numDropoff; ++i)
            dropoffBool[i] = dropoffStates[i] < WorldConfig::instance()->dropoffCapacity;

        auto applicableOps = applicableOperators(it->id);

        if(applicableOps.size() > 0) {
            RLState prevState = it->currentState(pickupBool, dropoffBool);
            Operator op = it->nextOp(prevState, applicableOps);
            applyOperator(op, it->id);

            for(int i = 0; i < numPickup; ++i)
                pickupBool[i] = pickupStates[i] != 0;
            for(int i = 0; i < numDropoff; ++i)
                dropoffBool[i] = dropoffStates[i] < WorldConfig::instance()->dropoffCapacity;
            RLState currState = it->currentState(pickupBool, dropoffBool);
            applicableOps = applicableOperators(it->id, true);
            it->updateTable(prevState, currState, op, applicableOps);
        }
    }
    bool final = isFinalState();
    if(final) timesCompleted++;
    if(resetOnFinal && final) reset();
    return final;
}

void
World::
resetAll() {
    // std::cout << "World::" << __func__ << " ENTER\n";
    stepsRun = 0;
    timesCompleted = 0;
    reset(true);
}

void
World::
reset(bool clearTable) {   
    // std::cout << "World::" << __func__ << " ENTER\n";
    for(auto it = agents.begin(); it != agents.end(); ++it)
        it->reset(clearTable);

    for(auto it = pickupStates.begin(); it != pickupStates.end(); ++it)
        *it = WorldConfig::instance()->pickupCapacity;
    
    for(auto it = dropoffStates.begin(); it != dropoffStates.end(); ++it)
        *it = 0;
}

bool
World::
isFinalState() {
    // std::cout << "World::" << __func__ << " ENTER\n";
    bool dropoffFull = true;
    for(auto it = dropoffStates.begin(); it != dropoffStates.end(); ++it)
        if(*it < WorldConfig::instance()->dropoffCapacity) {
            dropoffFull = false;
            break;
        }

    bool pickupEmpty = true;
    for(auto it = pickupStates.begin(); it != pickupStates.end(); ++it)
        if(*it > 0) {
            pickupEmpty = false;
            break;
        }
    
    bool agentHolding = false;
    for(auto it = agents.begin(); it != agents.end(); ++it)
        if(it->carryingBlock) {
            agentHolding = true;
            break;
        }

    return dropoffFull || (pickupEmpty && !agentHolding);
}

std::vector<Operator>
World::
applicableOperators(int agentID, bool ignoreAgents) {
    // std::cout << "World::" << __func__ << " ENTER\n";
    std::vector<Operator> applicableOps;
    Position agentPos = agents[agentID].pos;

    if(ignoreAgents) {
        if(agentPos.y > 0) 
            applicableOps.push_back(Operator::N);
        if(agentPos.x < WorldConfig::instance()->worldSize - 1)
            applicableOps.push_back(Operator::E);
        if(agentPos.y < WorldConfig::instance()->worldSize - 1)
            applicableOps.push_back(Operator::S);
        if(agentPos.x > 0) 
            applicableOps.push_back(Operator::W);
    }
    else {
        bool nFree = true;
        bool sFree = true;
        bool wFree = true;
        bool eFree = true;
        for(int i = 0; i < agents.size(); ++i) {
            if(agentID != i) {
                Position otherPos = agents[i].pos;
                if(otherPos.y - agentPos.y == -1 && otherPos.x == agentPos.x) nFree = false;
                if(otherPos.x - agentPos.x == 1 && otherPos.y == agentPos.y) eFree = false;
                if(otherPos.y - agentPos.y == 1 && otherPos.x == agentPos.x) sFree = false;
                if(otherPos.x - agentPos.x == -1 && otherPos.y == agentPos.y) wFree = false;
            }
        }
        if(nFree && agentPos.y > 0) 
            applicableOps.push_back(Operator::N);
        if(eFree && agentPos.x < WorldConfig::instance()->worldSize - 1)
            applicableOps.push_back(Operator::E);
        if(sFree && agentPos.y < WorldConfig::instance()->worldSize - 1)
            applicableOps.push_back(Operator::S);
        if(wFree && agentPos.x > 0) 
            applicableOps.push_back(Operator::W);
    }

    for(int i = 0; i < WorldConfig::instance()->pickupPositions.size(); ++i) {
        if(agentPos == WorldConfig::instance()->pickupPositions[i]) {
            if(pickupStates[i] > 0 && !agents[agentID].carryingBlock) 
                applicableOps.push_back(Operator::P);
            break;
        }
    }

    for(int i = 0; i < WorldConfig::instance()->dropoffPositions.size(); ++i) {
        if(agentPos == WorldConfig::instance()->dropoffPositions[i]) {
            if(dropoffStates[i] < WorldConfig::instance()->dropoffCapacity && agents[agentID].carryingBlock) 
                applicableOps.push_back(Operator::D);
            break;
        }
    }

    return applicableOps;
}

void
World::
applyOperator(Operator op, int agentID) {
    // std::cout << "World::" << __func__ << " ENTER\n";
    agents[agentID].applyOperator(op);
    if(op == Operator::D) {
        for(int i = 0; i < WorldConfig::instance()->dropoffPositions.size(); ++i)
            if(agents[agentID].pos == WorldConfig::instance()->dropoffPositions[i])
                dropoffStates[i] = dropoffStates[i] + 1;
    }
    if(op == Operator::P) {
        for(int i = 0; i < WorldConfig::instance()->pickupPositions.size(); ++i)
            if(agents[agentID].pos == WorldConfig::instance()->pickupPositions[i])
                pickupStates[i] = pickupStates[i] - 1;
    }
}

void
World::
print(bool printAgents) {
    if(printAgents) {
        std::cout << "AGENTS:\n";
        std::cout << "------------------------------" << "\n";
        for(auto it = agents.begin(); it != agents.end(); ++it) 
            it->print();
        std::cout << "------------------------------" << "\n";
    }

    std::cout << "PICKUP_STATES    : " << "[ ";
    for(auto state: pickupStates)
        std::cout << state << (state < 10 ? "  " : " ");
    std::cout << "]\n";

    std::cout << "DROPOFF_STATES   : " << "[ ";
    for(auto state: dropoffStates)
        std::cout << state << (state < 10 ? "  " : " ");
    std::cout << "]\n";

    std::cout << "STEPS_RUN        : " << stepsRun << "\n";
    std::cout << "TIMES_COMPLETED  : " << timesCompleted << "\n\n";
    
}

World::
~World() {
    delete singleton;
}
