#pragma once
#include <random>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
class FusionReaction
{
public:
    /**
     * @brief Factory method for Deuterium-Deuterium reaction.
     * @return A FusionReaction instance for DD reaction.
     */
    static FusionReaction DeuteriumDeuterium();

    /**
     * @brief Factory method for Deuterium-Tritium reaction.
     * @return A FusionReaction instance for DT reaction.
     */
    static FusionReaction DeuteriumTritium();

    /**
     * @brief Simulate a fusion event.
     * @return The energy released in the fusion event.
     */
    double simulateEvent();
private:
    double cross_section{};
    double Q_value{};
    std::mt19937 rng;
};
