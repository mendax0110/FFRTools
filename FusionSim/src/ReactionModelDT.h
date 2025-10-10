#pragma once
#include "IReactionModel.h"
#include <random>
#include "ParticleModelSFPS.h"

class ReactionModelDT : public IReactionModel
{
public:
    std::vector<std::unique_ptr<IParticleModel>> react(const std::vector<std::unique_ptr<IParticleModel>>& reactants) override
    {
        // D + T -> He-4 (3.5 MeV) + n (14.1 MeV)
        std::vector<std::unique_ptr<IParticleModel>> products;
        if (reactants.size() < 2) return products;
        Vector3d pos = (reactants[0]->getPosition() + reactants[1]->getPosition()) * 0.5;
        static std::mt19937 rng{43};
        std::uniform_real_distribution<double> phi(0, 2*M_PI);
        std::uniform_real_distribution<double> costheta(-1, 1);
        double theta = acos(costheta(rng));
        double phival = phi(rng);
        Vector3d dir1(sin(theta)*cos(phival), sin(theta)*sin(phival), cos(theta));
        Vector3d dir2 = -dir1;
        double neutron_mass = 1.67492749804e-27;
        double neutron_energy = 14.1e6 * 1.60218e-19;
        double neutron_speed = sqrt(2*neutron_energy/neutron_mass);
        double he4_mass = 6.646476e-27;
        double he4_energy = 3.5e6 * 1.60218e-19;
        double he4_speed = sqrt(2*he4_energy/he4_mass);
        products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir1*neutron_speed, neutron_mass, 0.0, nullptr, nullptr));
        products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir2*he4_speed, he4_mass, 2.0*1.60218e-19, nullptr, nullptr));
        return products;
    }
    std::string getName() const override { return "Deuterium-Tritium"; }
};
