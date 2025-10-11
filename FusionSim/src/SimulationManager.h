#pragma once
#include <memory>
#include <vector>
#include "IFieldModel.h"
#include "IReactionModel.h"
#include "IParticleModel.h"

class SimulationManager
{
public:
    void setFieldModel(std::unique_ptr<IFieldModel> model);
    void setReactionModel(std::unique_ptr<IReactionModel> model);
    void addParticle(std::unique_ptr<IParticleModel> particle);
    void run(double t_max, double dt);
    const std::vector<std::unique_ptr<IParticleModel>>& getParticles() const;
    IFieldModel* getFieldModel() const;
private:
    std::unique_ptr<IFieldModel> fieldModel;
    std::unique_ptr<IReactionModel> reactionModel;
    std::vector<std::unique_ptr<IParticleModel>> particles;
};
