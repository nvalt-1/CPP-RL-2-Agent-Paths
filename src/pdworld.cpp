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
    World* world = World::instance();
    CLIDisplay* display = CLIDisplay::instance();

    config->policy = Policy::PRANDOM;
    // config->print();
    for(int i = 0; i < 500; ++i) {
        world->step();
        if(i % 5 == 0) 
            display->draw();
    }

    world->reset();
    config->policy = Policy::PGREEDY;
    // config->print();
    for(int i = 0; i < 7500; ++i) {
        world->step();
        if(i % 5 == 0 || i > 7000) 
            display->draw();
    }
    std::cout << "FINAL:\n";
    std::cout << "==============================" << "\n";
    world->print();
    std::cout << "==============================" << "\n";
    return 0;
}