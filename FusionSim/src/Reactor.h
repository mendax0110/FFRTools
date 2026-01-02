#pragma once
#include "FusionReaction.h"
#include <vector>

/// @brief Class Representing Simple Reactor. \class Reactor
class Reactor
{
public:
    /**
     * @brief Setup for simple DD Fusion.
     */
    void setup_DD();

    /**
     * @brief Setup for simple DT Fusion.
     */
    void setup_DT();

    /**
     * @brief Start simulations..
     */
    void simulate();

    /**
     * @brief Getter for the Results.
     * @return A Vector of doubles representing the results.
     */
    [[nodiscard]] std::vector<double> getResults() const;
private:
    FusionReaction reaction;
    std::vector<double> results;
};
