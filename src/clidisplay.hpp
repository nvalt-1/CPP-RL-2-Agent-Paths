#ifndef CLIDISPLAY
#define CLIDISPLAY

#include "position.hpp"
#include "worldconfig.hpp"
#include "rlstate.hpp"
#include "agent.hpp"
#include "world.hpp"

#include <string>
#include <iostream>

class CLIDisplay {
public:
    static CLIDisplay* instance();
    
    std::string agentChars;
    std::string altAgentChars;

    void draw();

private:
    static CLIDisplay* singleton;

    std::string sep;

    CLIDisplay();
    CLIDisplay(const CLIDisplay&);
    CLIDisplay& operator=(const CLIDisplay&);
    ~CLIDisplay();
};

#endif