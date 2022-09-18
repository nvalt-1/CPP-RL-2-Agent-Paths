#ifndef QTABLE
#define QTABLE

#include "worldconfig.hpp"
#include "rlstate.hpp"

#include <algorithm>
#include <vector>
#include <iostream>

class QTable {
public:
    Operator nextOp(RLState state, std::vector<Operator> applicableOps);
    void update(RLState prevState, RLState currState, Operator op, std::vector<Operator> applicableOps);
    void reset();
    std::vector<double> QValues(RLState state);
    void print();

    QTable();

private:
    std::vector<std::vector<double>> table;
    std::vector<std::size_t> bases;

    std::size_t encodeState(RLState state);
    RLState decodeIndex(std::size_t index);
};

#endif