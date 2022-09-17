#ifndef CLIDISPLAY
#define CLIDISPLAY

#include "position.hpp"
#include "worldconfig.hpp"
#include "rlstate.hpp"
#include "agent.hpp"
#include "world.hpp"

#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

class CLIDisplay {
public:
    static CLIDisplay* instance();
    
    std::string agentChars;
    std::string altAgentChars;

    void draw(bool slow = false);

    #ifdef _WIN32
    void cls();
    #endif

private:
    static CLIDisplay* singleton;

    std::string sep;

    CLIDisplay();
    CLIDisplay(const CLIDisplay&);
    CLIDisplay& operator=(const CLIDisplay&);
    ~CLIDisplay();

    #ifdef _WIN32
    void setCursorPosition(int x, int y);
    #endif
};

#endif