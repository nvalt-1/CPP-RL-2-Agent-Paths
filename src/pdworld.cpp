#include "world.hpp"
#include "clidisplay.hpp"

#include <iostream>
#include <ctime>
#include <unordered_set>

WorldConfig* WorldConfig::singleton = nullptr;
World* World::singleton = nullptr;
CLIDisplay* CLIDisplay::singleton = nullptr;

int main(int argc, char* argv[]) {
    std::cout << "Enter seed #\n";
    std::string seedStr;
    std::getline(std::cin, seedStr);
    unsigned int seed;
    try {
        seed = std::stoi(seedStr);
        srand(seed);
        std::cout << "Using seed: " << seed << "\n";
    }
    catch (const std::invalid_argument & e) {
        seed = time(0);
        srand(seed);
        std::cout << "Using seed: " << seed << "\n";
    }
    catch (const std::out_of_range & e) {
        seed = time(0);
        srand(seed);
        std::cout << "Using seed: " << seed << "\n";
    }

    WorldConfig* config = WorldConfig::instance();
    CLIDisplay* display = CLIDisplay::instance();
    display->seed = seed;

    std::cout << "Creating agents...\n";
    config->worldSize = 5;
    int numAgents = 2;
    std::unordered_set<Position, PositionHash> agentPos;
    while(agentPos.size() < numAgents)
        agentPos.insert(Position(rand() % config->worldSize, rand() % config->worldSize));
    config->agentStartPositions.clear();
    for(auto it = agentPos.begin(); it != agentPos.end(); ++it)
        config->agentStartPositions.push_back(*it);

    std::cout << "Creating pickup and dropoff positions...\n";
    int numDropoff = 4;
    int numPickup = 2;
    std::unordered_set<Position, PositionHash> dropPickPos;
    while(dropPickPos.size() < numDropoff + numPickup)
        dropPickPos.insert(Position(rand() % config->worldSize, rand() % config->worldSize));

    config->dropoffPositions.clear();
    config->pickupPositions.clear();
    int idx = 0;
    for(auto it = dropPickPos.begin(); it != dropPickPos.end(); ++it) {
        if(idx < numDropoff)
            config->dropoffPositions.push_back(*it);
        else
            config->pickupPositions.push_back(*it);
        ++idx;
    }

    std::cout << "Creating World...\n";
    World* world = World::instance();

    std::cout << "Press enter to start\n";
    std::cin.get();

    config->policy = Policy::PRANDOM;
    config->method = Method::SARSA;
    #ifdef _WIN32
    display->cls();
    #endif
    for(int i = 0; i < 10000; ++i) {
        world->step();
        if(i % 200 == 0) 
            display->draw();
    }

    world->reset();
    config->policy = Policy::PEXPLOIT;
    for(int i = 0; i < 30000; ++i) {
        world->step();
        if(i % 200 == 0) 
            display->draw();
    }

    world->reset();
    config->policy = Policy::PGREEDY;
    for(int i = 0; i < 100000; ++i) {
        world->step();
        if(i > 99800) 
            display->draw(true);
        else 
        if(i % 200 == 0)
            display->draw();
    }
    // std::cout << "FINAL:\n";
    std::cout << "==============================" << "\n";
    // world->print();
    std::cout << "==============================" << "\n";
    return 0;
}