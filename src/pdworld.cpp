#include "world.hpp"
#include "clidisplay.hpp"

#include <iostream>
#include <ctime>

WorldConfig* WorldConfig::singleton = nullptr;
World* World::singleton = nullptr;
CLIDisplay* CLIDisplay::singleton = nullptr;

int main(int argc, char* argv[]) {
    srand(time(NULL));
    WorldConfig* config = WorldConfig::instance();
    CLIDisplay* display = CLIDisplay::instance();
    config->worldSize = 7;
    // display->promptConfig();
    config->agentStartPositions.push_back(Position(4,4));
    config->dropoffPositions.push_back(Position(0, 6));
    config->dropoffPositions.push_back(Position(5, 6));
    config->pickupPositions.push_back(Position(5, 0));

    World* world = World::instance();

    config->policy = Policy::PRANDOM;
    #ifdef _WIN32
    display->cls();
    #endif
    for(int i = 0; i < 10000; ++i) {
        world->step();
        if(i % 100 == 0) 
            display->draw();
    }

    world->reset();
    config->policy = Policy::PEXPLOIT;
    for(int i = 0; i < 30000; ++i) {
        world->step();
        if(i % 100 == 0) 
            display->draw();
    }

    world->reset();
    config->policy = Policy::PGREEDY;
    for(int i = 0; i < 100000; ++i) {
        world->step();
        if(i > 99800) 
            display->draw(true);
        else 
        if(i % 100 == 0)
            display->draw();
    }
    std::cout << "FINAL:\n";
    std::cout << "==============================" << "\n";
    world->print();
    std::cout << "==============================" << "\n";
    return 0;
}