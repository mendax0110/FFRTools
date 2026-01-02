#pragma once
#include <memory>
#include <vector>
#include <random>
#include <atomic>
#include "IFieldModel.h"
#include "IMagneticFieldModel.h"
#include "IReactionModel.h"
#include "IParticleModel.h"

#ifdef USE_OPENMP
#include <omp.h>
#endif

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Manages the Simulations. \class SimulationManager
    class SimulationManager
    {
    public:
        /**
         *  @brief Simple Ctor.
         */
        SimulationManager();

        /**
         * @brief Setter for hte Fieldmodel
         * @param model The model.
         */
        void setFieldModel(std::shared_ptr<IFieldModel> model);

        /**
         * @brief Setter for the Magneticfieldmodel
         * @param model The model.
         */
        void setMagneticFieldModel(std::shared_ptr<IMagneticFieldModel> model);

        /**
         * @brief Setter for the Reactionmodel.
         * @param model The model.
         */
        void setReactionModel(std::unique_ptr<IReactionModel> model);

        /**
         * @brief Setter for the Particlemodel.
         * @param particle The model.
         */
        void addParticle(std::unique_ptr<IParticleModel> particle);

        /**
         * @brief Setter for the Particle density.
         * @param density The density to set.
         */
        void setParticleDensity(double density);

        /**
         * @brief Setter for the Collisionradius.
         * @param radius The radius to set.
         */
        void setCollisionRadius(double radius);

        /**
         * @brief Setter fot the n_threads, to improved performance while running simulation.
         * @param threads The number of threads.
         */
        void setNumThreads(int threads);

        /**
         * @brief Entry method to run the simuation.
         * @param t_max
         * @param dt
         */
        void run(double t_max, double dt);

        /**
         * @brief Getter for the Particles.
         * @return A vector unqPtrs of Particlemodels.
         */
        [[nodiscard]] const std::vector<std::unique_ptr<IParticleModel>>& getParticles() const;

        /**
         * @brief Getter for the Fieldmodel.
         * @return A sharedPtr Fieldmodel.
         */
        [[nodiscard]] std::shared_ptr<IFieldModel> getFieldModel() const;

        /**
         * @brief Getter for the Magneticfieldmodel.
         * @return A sharedPtr Magneticfieldmodel
         */
        [[nodiscard]] std::shared_ptr<IMagneticFieldModel> getMagneticFieldModel() const;

        /**
         * @brief Getter for the reactions.
         * @return Count of reactions.
         */
        [[nodiscard]] size_t getReactionCount() const;

        /**
         * @brief Getter for the number of threads.
         * @return Number of threads.
         */
        [[nodiscard]] int getNumThreads() const;

    private:
        std::shared_ptr<IFieldModel> m_fieldModel;
        std::shared_ptr<IMagneticFieldModel> m_magFieldModel;
        std::unique_ptr<IReactionModel> m_reactionModel;
        std::vector<std::unique_ptr<IParticleModel>> m_particles;
        std::mt19937 m_rng;
        double m_particleDensity;
        double m_collisionRadius;
        std::atomic<size_t> m_reactionCount;
        int m_numThreads;
    };
}