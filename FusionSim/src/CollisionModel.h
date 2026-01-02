#pragma once
#include "IParticleModel.h"
#include <vector>
#include <memory>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Collision Model handling particle collisions. \class CollisionModel
    class CollisionModel
    {
    public:

        /**
         * @brief Handle elastic collision between two particles.
         * @param p1 First particle.
         * @param p2 Second particle.
         */
        static void elasticCollision(IParticleModel& p1, IParticleModel& p2);

        /**
         * @brief Handle inelastic collision between two particles with energy loss.
         * @param p1 First particle.
         * @param p2 Second particle.
         * @param energyLoss Energy lost during the collision.
         */
        static void inelasticCollision(IParticleModel& p1, IParticleModel& p2, double energyLoss);
    };
}
