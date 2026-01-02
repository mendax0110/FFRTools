#pragma once
#include "IReactionModel.h"
#include "ParticleModelSFPS.h"
#include "PhysicalConstants.h"
#include <random>
#include <cmath>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    //// @brief Simulation of the DT Reaction. \class ReactionModelDT
    class ReactionModelDT : public IReactionModel
    {
    public:

        /**
         * @brief Getter for the cross section.
         * @param energy_keV The energy in keV.
         * @return A double represting the cross section.
         */
        double getCrossSection(const double energy_keV) const override
        {
            if (energy_keV <= 0.0)
            {
                return 0.0;
            }

            using namespace constants::dt_reaction;

            const double E = energy_keV;
            const double denom = 1.0 + E * (A2 + E * (A4 + E * A5));
            const double S = A1 + (A3 * E) / denom;
            const double sigma_mb = S / (E * std::exp(BG / std::sqrt(E)));

            return sigma_mb * constants::millibarn;
        }

        /**
         * @brief Method to start reaction.
         * @param reactants The particle model.
         * @param fieldModel The field model.
         * @param magFieldModel The magnetic field model.
         * @return A vector of unqPtrs with the react.
         */
        std::vector<std::unique_ptr<IParticleModel>> react(
            const std::vector<std::unique_ptr<IParticleModel>>& reactants,
            std::shared_ptr<const IFieldModel> fieldModel,
            std::shared_ptr<const IMagneticFieldModel> magFieldModel) override
        {
            std::vector<std::unique_ptr<IParticleModel>> products;
            if (reactants.size() < 2)
            {
                return products;
            }

            const Vector3d pos = (reactants[0]->getPosition() + reactants[1]->getPosition()) * 0.5;

            std::uniform_real_distribution<> phiDist(0, 2 * constants::pi);
            std::uniform_real_distribution<> cosThetaDist(-1, 1);

            const double theta = std::acos(cosThetaDist(m_rng));
            const double phival = phiDist(m_rng);
            const Vector3d dir1(
                std::sin(theta) * std::cos(phival),
                std::sin(theta) * std::sin(phival),
                std::cos(theta));
            const Vector3d dir2 = -dir1;

            constexpr double neutronEnergy = constants::dt_reaction::E_neutron * constants::MeVtoJoule;
            const double neutronSpeed = std::sqrt(2.0 * neutronEnergy / constants::massNeutron);

            constexpr double he4Energy = constants::dt_reaction::E_He4 * constants::MeVtoJoule;
            const double he4Speed = std::sqrt(2.0 * he4Energy / constants::massHe4);

            products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir1 * neutronSpeed, constants::massNeutron, 0.0, nullptr, nullptr));
            products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir2 * he4Speed, constants::massHe4, 2.0 * constants::eCharge, fieldModel, magFieldModel));

            return products;
        }

        /**
         * @brief Getter for the Name of the react.
         * @return A String represnting the name.
         */
        std::string getName() const override
        {
            return "Deuterium-Tritium";
        }

    private:
        mutable std::mt19937 m_rng{std::random_device{}()};
    };
}