#ifndef QTABLE
#define QTABLE

#include "rlstate.hpp"
#include "worldconfig.hpp"
#include <vector>
#include <algorithm>

class QTable {
public:
    WorldConfig::Operator nextOp(RLState, std::vector<WorldConfig::Operator>);
    void update(RLState, RLState, WorldConfig::Operator, std::vector<WorldConfig::Operator>);
    void reset();
    std::vector<double> QValues(RLState);

    QTable();
private:
    std::vector<std::vector<double>> table;
    std::vector<int> bases;

    int encodeState(RLState);
    RLState decodeIndex(int);
};

#endif