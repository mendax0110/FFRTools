#include "SimulationManager.h"
#include "PhysicalConstants.h"
#include "FarnsworthFusorFieldModel.h"
#include <cmath>
#include <iostream>
#include <vector>

#ifdef USE_OPENMP
#include <omp.h>
#endif

using namespace fusion;

namespace
{
    inline void indexToPair(const size_t k, const size_t n, size_t& i, size_t& j)
    {
        i = n - 2 - static_cast<size_t>(std::floor(std::sqrt(static_cast<double>(-8 * k + 4 * n * (n - 1) - 7)) / 2.0 - 0.5));
        j = k + i + 1 - n * (n - 1) / 2 + (n - i) * ((n - i) - 1) / 2;
    }
}


SimulationManager::SimulationManager()
    : m_rng(std::random_device{}())
    , m_particleDensity(1.0e19)
    , m_collisionRadius(1.0e-3)
    , m_reactionCount(0)
    , m_numThreads(1)
    , m_thermalModel(nullptr)
    , m_enableThermalDynamics(false)
{
#ifdef USE_OPENMP
    m_numThreads = omp_get_max_threads();
#endif
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

void SimulationManager::setParticleDensity(double density)
{
    m_particleDensity = density;
}

void SimulationManager::setCollisionRadius(double radius)
{
    m_collisionRadius = radius;
}

void SimulationManager::setNumThreads(int threads)
{
    m_numThreads = threads;
#ifdef USE_OPENMP
    omp_set_num_threads(threads);
#endif
}

int SimulationManager::getNumThreads() const
{
    return m_numThreads;
}

template <typename RNG, typename OutputIt>
void SimulationManager::processPair(const size_t i, const size_t j, const double dt, RNG& rng, OutputIt out)
{
    const Vector3d dr = m_particles[i]->getPosition() - m_particles[j]->getPosition();
    const double distance = dr.norm();
    if (distance > m_collisionRadius)
    {
        return;
    }

    const Vector3d vRel = m_particles[i]->getVelocity() - m_particles[j]->getVelocity();
    const double v = vRel.norm();

    const double m1 = m_particles[i]->getMass();
    const double m2 = m_particles[j]->getMass();
    const double reducedMass = (m1 * m2) / (m1 + m2);

    const double E_cm_J = 0.5 * reducedMass * v * v;
    const double E_cm_keV = E_cm_J / constants::keVtoJoule;

    const double sigma = m_reactionModel->getCrossSection(E_cm_keV);
    const double prob = sigma * v * dt * m_particleDensity;

    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    if (uniform(rng) < prob)
    {
        std::vector<std::unique_ptr<IParticleModel>> reactants;
        reactants.push_back(m_particles[i]->clone());
        reactants.push_back(m_particles[j]->clone());

        auto products = m_reactionModel->react(
            reactants,
            m_fieldModel,
            m_magFieldModel);

        for (auto& p : products)
            *out++ = std::move(p);

#ifdef USE_OPENMP
        m_reactionCount.fetch_add(1, std::memory_order_relaxed);
#else
        ++m_reactionCount;
#endif
    }
}

void SimulationManager::run(const double t_max, double dt)
{
    double t = 0.0;
    size_t step = 0;
    m_reactionCount = 0;

    auto* fusorField = dynamic_cast<FarnsworthFusorFieldModel*>(m_fieldModel.get());

#ifdef USE_OPENMP
    std::cout << "Running with " << m_numThreads << " OpenMP threads\n";
    std::vector<std::mt19937> threadRngs(m_numThreads);
    for (int i = 0; i < m_numThreads; ++i)
        threadRngs[i].seed(std::random_device{}() + i * 1000);
#else
    std::cout << "Running single-threaded\n";
#endif

    while (t < t_max)
    {
        const size_t n = m_particles.size();

        if (fusorField && m_enableThermalDynamics && m_thermalModel && step % 100 == 0)
        {
            double avgKE = 0.0, totalSpeed = 0.0;
            for (const auto& p : m_particles)
            {
                const double v2 = p->getVelocity().squaredNorm();
                avgKE += 0.5 * p->getMass() * v2;
                totalSpeed += std::sqrt(v2);
            }
            avgKE /= n;
            const double avgSpeed = totalSpeed / n;

            const double gridRadius = fusorField->getInnerGridRadius();
            const double gridArea = 4.0 * constants::pi * gridRadius * gridRadius;
            const double flux = m_particleDensity * avgSpeed * gridArea / 4.0;
            const double current = flux * constants::eCharge;

            m_thermalModel->updateTemperatures(
                dt * 100.0,
                current,
                flux,
                avgKE,
                fusorField->getOperatingPressure());

            fusorField->setGridTemperature(m_thermalModel->getGridTemperature());
            fusorField->setChamberTemperature(m_thermalModel->getChamberTemperature());
        }

#ifdef USE_OPENMP
        #pragma omp parallel for schedule(static)
#endif
        for (long long i = 0; i < static_cast<long long>(n); ++i)
        {
            m_particles[i]->propagate(dt);
        }

        std::vector<std::unique_ptr<IParticleModel>> newParticles;

        if (n >= 2)
        {
            const size_t numPairs = n * (n - 1) / 2;

#ifdef USE_OPENMP
            std::vector<std::vector<std::unique_ptr<IParticleModel>>> locals(m_numThreads);

            #pragma omp parallel
            {
                int tid = omp_get_thread_num();
                auto& rng = threadRngs[tid];
                auto& local = locals[tid];

                #pragma omp for schedule(static)
                for (long long k = 0; k < static_cast<long long>(numPairs); ++k)
                {
                    size_t i, j;
                    indexToPair(k, n, i, j);
                    processPair(i, j, dt, rng, std::back_inserter(local));
                }
            }

            for (auto& v : locals)
            {
                for (auto& p : v)
                {
                    newParticles.push_back(std::move(p));
                }
            }
#else
            for (size_t k = 0; k < numPairs; ++k)
            {
                size_t i, j;
                indexToPair(k, n, i, j);
                processPair(i, j, dt, m_rng, std::back_inserter(newParticles));
            }
#endif
        }

        for (auto& p : newParticles)
        {
            m_particles.push_back(std::move(p));
        }

        t += dt;
        ++step;

        if (step % 1000 == 0)
        {
            std::cout << "\rProgress: "
                      << int(100.0 * t / t_max)
                      << "%  Particles: " << m_particles.size()
                      << "  Reactions: " << m_reactionCount
                      << std::flush;
        }
    }
    std::cout << "\n";
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

size_t SimulationManager::getReactionCount() const
{
    return m_reactionCount;
}

void SimulationManager::enableThermalDynamics(const bool enable)
{
    m_enableThermalDynamics = enable;
    if (enable && !m_thermalModel)
    {
        m_thermalModel = std::make_unique<ThermalDynamicsModel>();
    }
}

ThermalDynamicsModel* SimulationManager::getThermalModel() const
{
    return m_thermalModel.get();
}
