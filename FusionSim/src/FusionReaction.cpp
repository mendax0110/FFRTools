#include "FusionReaction.h"
#include <random>

FusionReaction FusionReaction::DeuteriumDeuterium()
{
    FusionReaction r;
    r.cross_section = 1.0e-28;
    r.Q_value = 4.03;
    r.rng.seed(42);
    return r;
}
FusionReaction FusionReaction::DeuteriumTritium()
{
    FusionReaction r;
    r.cross_section = 5.0e-28;
    r.Q_value = 17.6;
    r.rng.seed(43);
    return r;
}
double FusionReaction::simulateEvent()
{
    std::normal_distribution<> dist(Q_value, 0.1);
    return dist(rng);
}
