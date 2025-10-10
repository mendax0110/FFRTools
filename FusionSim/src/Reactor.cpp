#include "Reactor.h"
#include <iostream>

void Reactor::setup_DD()
{
    reaction = FusionReaction::DeuteriumDeuterium();
}

void Reactor::setup_DT()
{
    reaction = FusionReaction::DeuteriumTritium();
}

void Reactor::simulate()
{
    results.clear();
    // 1000 reaktionen simulieren
    for (int i = 0; i < 1000; ++i)
    {
        results.push_back(reaction.simulateEvent());
    }
    std::cout << "Simulation abgeschlossen: " << results.size() << " Ereignisse." << std::endl;
}
std::vector<double> Reactor::getResults() const
{
    return results;
}
