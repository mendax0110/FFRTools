#pragma once
#include <memory>
#include <vector>
#include <string>
#include "IParticleModel.h"

namespace fusion
{
    class IReactionModel
    {
    public:
        virtual ~IReactionModel() = default;

        [[nodiscard]] virtual double getCrossSection(double energy_keV) const = 0;

        virtual std::vector<std::unique_ptr<IParticleModel>> react(
            const std::vector<std::unique_ptr<IParticleModel>>& reactants,
            std::shared_ptr<const IFieldModel> fieldModel,
            std::shared_ptr<const IMagneticFieldModel> magFieldModel) = 0;

        [[nodiscard]] virtual std::string getName() const = 0;
    };
}
