#include "clidisplay.hpp"

CLIDisplay* 
CLIDisplay::
instance() {
    return (!singleton) ? singleton = new CLIDisplay() : singleton;
}

void
CLIDisplay::
draw(bool slow) {
    // std::cout << "CLIDisplay::" << __func__ << " ENTER\n";
    #ifndef _WIN32
    system("clear");
    #endif
    #ifdef _WIN32
    setCursorPosition(0, 0);
    #endif
    std::cout << "seed: " << seed << "\n";
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
    World::instance()->print(false);
    WorldConfig::instance()->print();
    std::cout.flush(); 

    if(slow) {
        #ifdef _WIN32
        Sleep(75);
        #endif
        #ifndef _WIN32
        usleep(75000);
        #endif
    }
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

#ifdef _WIN32
void 
CLIDisplay::
cls() {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD topLeft = { 0, 0 };
    std::cout.flush();
    if(!GetConsoleScreenBufferInfo(hOut, &csbi)) {
        abort();
    }
    DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;

    // Flood-fill the console with spaces to clear it
    FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

    // Reset the attributes of every character to the default.
    // This clears all background colour formatting, if any.
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

    // Move the cursor back to the top left for the next sequence of writes
    SetConsoleCursorPosition(hOut, topLeft);
}

void 
CLIDisplay::
setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}
#endif
