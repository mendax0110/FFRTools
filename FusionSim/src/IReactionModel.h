#pragma once
#include <memory>
#include <vector>
#include <string>
#include "IParticleModel.h"

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Interface for Reaction Models. \class IReactionModel
    class IReactionModel
    {
    public:

        /**
         * @brief Virtual destructor for IReactionModel.
         */
        virtual ~IReactionModel() = default;

        /**
         * @brief Getter for the cross section.
         * @param energy_keV The energy in keV.
         * @return A double represting the cross section.
         */
        [[nodiscard]] virtual double getCrossSection(double energy_keV) const = 0;

        /**
         * @brief Method to start reaction.
         * @param reactants The particle model.
         * @param fieldModel The field model.
         * @param magFieldModel The magnetic field model.
         * @return A vector of unqPtrs with the react.
         */
        virtual std::vector<std::unique_ptr<IParticleModel>> react(
            const std::vector<std::unique_ptr<IParticleModel>>& reactants,
            std::shared_ptr<const IFieldModel> fieldModel,
            std::shared_ptr<const IMagneticFieldModel> magFieldModel) = 0;

        /**
         * @brief Getter for the name of the reaction model.
         * @return The name as string.
         */
        [[nodiscard]] virtual std::string getName() const = 0;
    };
}
