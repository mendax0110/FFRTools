#pragma once
#include "IReactionModel.h"
#include "ParticleModelSFPS.h"
#include <random>
#include <cmath>

namespace fusion
{
    class ReactionModelDT : public IReactionModel
    {
    public:
        double getCrossSection(const double energy_keV) const override
        {
            if (energy_keV <= 0.0)
            {
                return 0.0;
            }
            constexpr double BG = 34.3827;
            constexpr double A1 = 6.927e4;
            constexpr double A2 = 7.454e8;
            constexpr double A3 = 2.050e6;
            constexpr double A4 = 5.2002e4;
            constexpr double A5 = 0.0;
            const double denom = 1.0 + (A2 + (A4 + A5 * energy_keV) * energy_keV) * energy_keV;
            const double S = A1 + (A3 * energy_keV) / denom;
            const double sigma = (S / (energy_keV * std::exp(BG / std::sqrt(energy_keV)))) * 1e-28;
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

            std::uniform_real_distribution<double> phiDist(0, 2 * M_PI);
            std::uniform_real_distribution<double> cosThetaDist(-1, 1);
            const double theta = std::acos(cosThetaDist(m_rng));
            const double phival = phiDist(m_rng);
            const Vector3d dir1(
                std::sin(theta) * std::cos(phival),
                std::sin(theta) * std::sin(phival),
                std::cos(theta));
            const Vector3d dir2 = -dir1;

            constexpr double neutronMass = 1.67492749804e-27;
            constexpr double neutronEnergy = 14.1e6 * 1.60218e-19;
            const double neutronSpeed = std::sqrt(2 * neutronEnergy / neutronMass);

            constexpr double he4Mass = 6.646476e-27;
            constexpr double he4Energy = 3.5e6 * 1.60218e-19;
            const double he4Speed = std::sqrt(2 * he4Energy / he4Mass);

            constexpr double he4Charge = 2.0 * 1.60218e-19;

            products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir1 * neutronSpeed, neutronMass, 0.0, nullptr, nullptr));

            products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir2 * he4Speed, he4Mass, he4Charge, fieldModel, magFieldModel));

            return products;
        }

        std::string getName() const override
        {
            return "Deuterium-Tritium";
        }

    private:
        mutable std::mt19937 m_rng{std::random_device{}()};
    };
}