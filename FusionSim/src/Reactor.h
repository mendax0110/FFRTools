#pragma once
#include "FusionReaction.h"
#include <vector>

class Reactor
{
public:
    void setup_DD();
    void setup_DT();
    void simulate();
    [[nodiscard]] std::vector<double> getResults() const;
private:
    FusionReaction reaction;
    std::vector<double> results;
};
