#pragma once
#include "IReactionModel.h"
#include "ParticleModelSFPS.h"
#include <random>
#include <cmath>

namespace fusion
{
    class ReactionModelDD : public IReactionModel
    {
    public:
        double getCrossSection(const double energy_keV) const override
        {
            if (energy_keV <= 0.0)
            {
                return 0.0;
            }
            constexpr double S = 55.49;
            double sigma = S * std::exp(-std::sqrt(46.0 / energy_keV)) / energy_keV;
            sigma *= 1e-28;
            return sigma;
        }

        std::vector<std::unique_ptr<IParticleModel>> react(const std::vector<std::unique_ptr<IParticleModel>>& reactants,
                                                            std::shared_ptr<const IFieldModel> fieldModel,
                                                            std::shared_ptr<const IMagneticFieldModel> magFieldModel) override
        {
            std::vector<std::unique_ptr<IParticleModel>> products;
            if (reactants.size() < 2)
            {
                return products;
            }

            Vector3d pos = (reactants[0]->getPosition() + reactants[1]->getPosition()) * 0.5;

            std::uniform_real_distribution<> phiDist(0, 2 * M_PI);
            std::uniform_real_distribution<> cosThetaDist(-1, 1);
            const double theta = std::acos(cosThetaDist(m_rng));
            const double phival = phiDist(m_rng);
            const Vector3d dir1(
                std::sin(theta) * std::cos(phival),
                std::sin(theta) * std::sin(phival),
                std::cos(theta));
            const Vector3d dir2 = -dir1;

            constexpr double neutronMass = 1.67492749804e-27;
            constexpr double neutronEnergy = 2.45e6 * 1.60218e-19;
            const double neutronSpeed = std::sqrt(2 * neutronEnergy / neutronMass);

            constexpr double he3Mass = 5.008234e-27;
            constexpr double he3Energy = 0.82e6 * 1.60218e-19;
            const double he3Speed = std::sqrt(2 * he3Energy / he3Mass);

            constexpr double he3Charge = 2.0 * 1.60218e-19;

            products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir1 * neutronSpeed, neutronMass, 0.0, nullptr, nullptr));

            products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir2 * he3Speed, he3Mass, he3Charge, fieldModel, magFieldModel));

            return products;
        }

        std::string getName() const override
        {
            return "Deuterium-Deuterium";
        }

    private:
        mutable std::mt19937 m_rng{std::random_device{}()};
    };
}
