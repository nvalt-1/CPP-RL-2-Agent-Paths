#include "clidisplay.hpp"

CLIDisplay* 
CLIDisplay::
instance() {
    return (!singleton) ? singleton = new CLIDisplay() : singleton;
}

void
CLIDisplay::
draw() {
    // std::cout << "CLIDisplay::" << __func__ << " ENTER\n";
    system("clear");
    for(int y = 0; y < WorldConfig::instance()->worldSize; ++y) {
        std::cout << "[";
        for(int x = 0; x < WorldConfig::instance()->worldSize; ++x) {
            Position pos(x, y);
            bool isPickup = false;
            for(int i = 0; i < WorldConfig::instance()->pickupPositions.size(); ++i) {
                if(pos == WorldConfig::instance()->pickupPositions[i]) {
                    isPickup = true;
                    break;
                }
            }

            bool isDropoff = false;
            for(int i = 0; i < WorldConfig::instance()->dropoffPositions.size(); ++i) {
                if(pos == WorldConfig::instance()->dropoffPositions[i]) {
                    isDropoff = true;
                    break;
                }
            }

            bool hasAgent = false;
            bool carryingBlock = false;
            int agentID = -1;
            for(int i = 0; i < World::instance()->agents.size(); ++i) {
                if(pos == World::instance()->agents[i].pos) {
                    hasAgent = true;
                    carryingBlock = World::instance()->agents[i].carryingBlock;
                    agentID = i;
                    break;
                }
            }

            std::cout << " " << (isPickup ? "P" : isDropoff ? "D" : " ") << 
                (hasAgent ? carryingBlock ? altAgentChars[agentID] : agentChars[agentID] : sep[0]) << 
                (x == WorldConfig::instance()->worldSize - 1 ? "" : "|");
        }
        std::cout << "]\n";
    }
    World::instance()->print();
}

CLIDisplay::
CLIDisplay() {
    agentChars = "abcefghijklmnoqrstuvwxyz";
    altAgentChars = "ABCEFGHIJKLMNOQRSTUVWXYZ";
    sep = " ";
}

CLIDisplay::
~CLIDisplay() {
    delete singleton;
}
