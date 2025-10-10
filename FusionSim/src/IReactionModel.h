#pragma once
#include <memory>
#include <vector>
#include "IParticleModel.h"

class IReactionModel
{
public:
    virtual ~IReactionModel() = default;
    virtual std::vector<std::unique_ptr<IParticleModel>> react(const std::vector<std::unique_ptr<IParticleModel>>& reactants) = 0;
    virtual std::string getName() const = 0;
};
