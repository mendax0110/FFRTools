#pragma once
#include "IReactionModel.h"
#include <random>
#include "ParticleModelSFPS.h"

class ReactionModelDD : public IReactionModel
{
public:
    std::vector<std::unique_ptr<IParticleModel>> react(const std::vector<std::unique_ptr<IParticleModel>>& reactants) override
    {
        // D + D -> He-3 (0.82 MeV) + n (2.45 MeV)
        std::vector<std::unique_ptr<IParticleModel>> products;
        if (reactants.size() < 2) return products;
        // Annahme: Reaktionsort ist Mittelwert der Positionen
        Vector3d pos = (reactants[0]->getPosition() + reactants[1]->getPosition()) * 0.5;
        // Zufällige Richtung für Produkte
        static std::mt19937 rng{42};
        std::uniform_real_distribution<double> phi(0, 2*M_PI);
        std::uniform_real_distribution<double> costheta(-1, 1);
        double theta = acos(costheta(rng));
        double phival = phi(rng);
        Vector3d dir1(sin(theta)*cos(phival), sin(theta)*sin(phival), cos(theta));
        Vector3d dir2 = -dir1;
        // energyerhaltung: impulgsrichtung zufällig, Betrag aus Energie
        double neutron_mass = 1.67492749804e-27; // kg
        double neutron_energy = 2.45e6 * 1.60218e-19; // J
        double neutron_speed = sqrt(2*neutron_energy/neutron_mass);
        double he3_mass = 5.008234e-27;
        double he3_energy = 0.82e6 * 1.60218e-19;
        double he3_speed = sqrt(2*he3_energy/he3_mass);
        products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir1*neutron_speed, neutron_mass, 0.0, nullptr, nullptr));
        products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir2*he3_speed, he3_mass, 2.0*1.60218e-19, nullptr, nullptr));
        return products;
    }
    std::string getName() const override { return "Deuterium-Deuterium"; }
};
