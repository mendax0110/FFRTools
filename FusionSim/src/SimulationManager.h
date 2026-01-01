#pragma once
#include <memory>
#include <vector>
#include <random>
#include "IFieldModel.h"
#include "IMagneticFieldModel.h"
#include "IReactionModel.h"
#include "IParticleModel.h"

namespace fusion
{
    class SimulationManager
    {
    public:
        SimulationManager();

        void setFieldModel(std::shared_ptr<IFieldModel> model);
        void setMagneticFieldModel(std::shared_ptr<IMagneticFieldModel> model);
        void setReactionModel(std::unique_ptr<IReactionModel> model);
        void addParticle(std::unique_ptr<IParticleModel> particle);
        void run(double t_max, double dt);

        [[nodiscard]] const std::vector<std::unique_ptr<IParticleModel>>& getParticles() const;
        [[nodiscard]] std::shared_ptr<IFieldModel> getFieldModel() const;
        [[nodiscard]] std::shared_ptr<IMagneticFieldModel> getMagneticFieldModel() const;

    private:
        std::shared_ptr<IFieldModel> m_fieldModel;
        std::shared_ptr<IMagneticFieldModel> m_magFieldModel;
        std::unique_ptr<IReactionModel> m_reactionModel;
        std::vector<std::unique_ptr<IParticleModel>> m_particles;
        std::mt19937 m_rng;
    };
}