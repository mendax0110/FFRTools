#include "SimulationManager.h"

void SimulationManager::setFieldModel(std::unique_ptr<IFieldModel> model)
{
    fieldModel = std::move(model);
}

void SimulationManager::setReactionModel(std::unique_ptr<IReactionModel> model)
{
    reactionModel = std::move(model);
}

void SimulationManager::addParticle(std::unique_ptr<IParticleModel> particle)
{
    particles.push_back(std::move(particle));
}

void SimulationManager::run(double t_max, double dt)
{
    double t = 0.0;
    while (t < t_max)
    {
        for (auto& p : particles)
        {
            p->propagate(dt);
        }

        std::vector<std::unique_ptr<IParticleModel>> new_particles;
        for (size_t i = 0; i < particles.size(); ++i)
        {
            for (size_t j = i+1; j < particles.size(); ++j)
            {
                // reaktionswahrscheindlickeit nach wirkungsquerschnitt
                Vector3d v_rel = particles[i]->getVelocity() - particles[j]->getVelocity();
                double v = v_rel.norm();
                // Wirkungsquerschnitt nach Bosch-Hale für D-)
                // E_rel in keV
                double E_rel = 0.5 * particles[i]->getMass() * v*v / 1.60218e-16;
                double S = 55.49; // keV*barn
                double sigma = S * exp(-sqrt(46.0/E_rel)) / (E_rel); // barn
                sigma *= 1e-28; // barn -> m^2
                double n = 1.0; // Normierung, kann Dichte sein
                double prob = sigma * v * dt * n;
                if (((double)rand()/RAND_MAX) < prob)
                {
                    auto products = reactionModel->react({particles[i]->clone(), particles[j]->clone()});
                    for (auto& prod : products) new_particles.push_back(std::move(prod));
                }
            }
        }
        for (auto& np : new_particles) particles.push_back(std::move(np));
        {
            t += dt;
        }
    }
}

const std::vector<std::unique_ptr<IParticleModel>>& SimulationManager::getParticles() const
{
    return particles;
}
