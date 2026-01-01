#include "SimulationManager.h"
#include <cmath>

using namespace fusion;

SimulationManager::SimulationManager()
    : m_rng(std::random_device{}())
{
}

void SimulationManager::setFieldModel(std::shared_ptr<IFieldModel> model)
{
    m_fieldModel = std::move(model);
}

void SimulationManager::setMagneticFieldModel(std::shared_ptr<IMagneticFieldModel> model)
{
    m_magFieldModel = std::move(model);
}

void SimulationManager::setReactionModel(std::unique_ptr<IReactionModel> model)
{
    m_reactionModel = std::move(model);
}

void SimulationManager::addParticle(std::unique_ptr<IParticleModel> particle)
{
    m_particles.push_back(std::move(particle));
}

void SimulationManager::run(const double t_max, const double dt)
{
    std::uniform_real_distribution<double> uniform(0.0, 1.0);
    double t = 0.0;

    while (t < t_max)
    {
        for (const auto& p : m_particles)
        {
            p->propagate(dt);
        }

        std::vector<std::unique_ptr<IParticleModel>> newParticles;

        for (size_t i = 0; i < m_particles.size(); ++i)
        {
            for (size_t j = i + 1; j < m_particles.size(); ++j)
            {
                Vector3d vRel = m_particles[i]->getVelocity() - m_particles[j]->getVelocity();
                const double v = vRel.norm();

                const double reducedMass = (m_particles[i]->getMass() * m_particles[j]->getMass()) / (m_particles[i]->getMass() + m_particles[j]->getMass());
                const double E_rel_keV = 0.5 * reducedMass * v * v / 1.60218e-16;

                const double sigma = m_reactionModel->getCrossSection(E_rel_keV);

                constexpr double particleDensity = 1.0e20;
                const double prob = sigma * v * dt * particleDensity;

                if (uniform(m_rng) < prob)
                {
                    std::vector<std::unique_ptr<IParticleModel>> reactants;
                    reactants.push_back(m_particles[i]->clone());
                    reactants.push_back(m_particles[j]->clone());

                    auto products = m_reactionModel->react(
                        reactants,
                        m_fieldModel,
                        m_magFieldModel);

                    for (auto& prod : products)
                    {
                        newParticles.push_back(std::move(prod));
                    }
                }
            }
        }

        for (auto& np : newParticles)
        {
            m_particles.push_back(std::move(np));
        }

        t += dt;
    }
}

const std::vector<std::unique_ptr<IParticleModel>>& SimulationManager::getParticles() const
{
    return m_particles;
}

std::shared_ptr<IFieldModel> SimulationManager::getFieldModel() const
{
    return m_fieldModel;
}

std::shared_ptr<IMagneticFieldModel> SimulationManager::getMagneticFieldModel() const
{
    return m_magFieldModel;
}
