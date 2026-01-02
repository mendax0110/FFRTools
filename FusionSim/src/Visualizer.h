#pragma once
#include <vector>
#include <string>
#include "IParticleModel.h"

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    //// @brief Visualizer helps plotting particle plots. \class Visualizer
    class Visualizer
    {
    public:
        /**
         * @brief Method to plot the particels.
         *
         * @param particles The Particlemodel.
         * @param filename The filename of the generated picture.
         */
        static void plot(const std::vector<std::unique_ptr<IParticleModel>>& particles, const std::string& filename = "fusion_particles.png");
    };
}
