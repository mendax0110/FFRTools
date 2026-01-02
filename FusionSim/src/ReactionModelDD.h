#pragma once
#include "IReactionModel.h"
#include "ParticleModelSFPS.h"
#include "PhysicalConstants.h"
#include <random>
#include <cmath>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Simulation of the DD Reaction. \class ReactionModelDD
    class ReactionModelDD : public IReactionModel
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

            using namespace constants::dd_reaction;

            const double E = energy_keV;
            const double E2 = E * E;
            const double E3 = E2 * E;
            const double E4 = E3 * E;

            const double S = A1 + E * (A2 + E * (A3 + E * (A4 + E * A5)));

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

            std::uniform_real_distribution<double> phiDist(0, 2 * constants::pi);
            std::uniform_real_distribution<double> cosThetaDist(-1, 1);
            std::uniform_real_distribution<double> branchDist(0, 1);

            const double theta = std::acos(cosThetaDist(m_rng));
            const double phival = phiDist(m_rng);
            const Vector3d dir1(
                std::sin(theta) * std::cos(phival),
                std::sin(theta) * std::sin(phival),
                std::cos(theta));
            const Vector3d dir2 = -dir1;

            const bool he3Branch = branchDist(m_rng) < 0.5;

            if (he3Branch)
            {
                constexpr double neutronEnergy = constants::dd_reaction::E_neutron_He3 * constants::MeVtoJoule;
                const double neutronSpeed = std::sqrt(2.0 * neutronEnergy / constants::massNeutron);

                constexpr double he3Energy = constants::dd_reaction::E_He3 * constants::MeVtoJoule;
                const double he3Speed = std::sqrt(2.0 * he3Energy / constants::massHe3);

                products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir1 * neutronSpeed, constants::massNeutron, 0.0, nullptr, nullptr));

                products.push_back(std::make_unique<ParticleModelSFPS>(pos, dir2 * he3Speed, constants::massHe3, 2.0 * constants::eCharge, fieldModel, magFieldModel));
            }
            else
            {
                constexpr double protonEnergy = constants::dd_reaction::E_proton * constants::MeVtoJoule;
                const double protonSpeed = std::sqrt(2.0 * protonEnergy / constants::massProton);

                constexpr double tritiumEnergy = constants::dd_reaction::E_Tritium * constants::MeVtoJoule;
                const double tritiumSpeed = std::sqrt(2.0 * tritiumEnergy / constants::massTritium);

                products.push_back(std::make_unique<ParticleModelSFPS>(
                    pos, dir1 * protonSpeed, constants::massProton,
                    constants::eCharge, fieldModel, magFieldModel));

                products.push_back(std::make_unique<ParticleModelSFPS>(
                    pos, dir2 * tritiumSpeed, constants::massTritium,
                    constants::eCharge, fieldModel, magFieldModel));
            }

            return products;
        }

        /**
         * @brief Getter for the Name of the react.
         * @return A String represnting the name.
         */
        std::string getName() const override
        {
            return "Deuterium-Deuterium";
        }

    private:
        mutable std::mt19937 m_rng{std::random_device{}()};
    };
}
