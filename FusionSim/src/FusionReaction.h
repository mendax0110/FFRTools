#pragma once
#include <random>

class FusionReaction
{
public:
    static FusionReaction DeuteriumDeuterium();
    static FusionReaction DeuteriumTritium();
    double simulateEvent();
private:
    double cross_section{};
    double Q_value{};
    std::mt19937 rng;
};
