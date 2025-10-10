#pragma once
#include <vector>
#include <string>

class Visualizer
{
public:
    static void plot(const std::vector<std::unique_ptr<IParticleModel>>& particles, const std::string& filename = "fusion_particles.png");
};
