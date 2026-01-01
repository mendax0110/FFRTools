#pragma once
#include "IParticleModel.h"
#include <vector>
#include <memory>

namespace fusion
{
    class CollisionModel
    {
    public:
        static void elasticCollision(IParticleModel& p1, IParticleModel& p2);
        static void inelasticCollision(IParticleModel& p1, IParticleModel& p2, double energyLoss);
    };
}
