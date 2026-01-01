#include "CLI.h"
#include "SimulationManager.h"
#include "FieldModelPotentialMap.h"
#include "ParticleModelSFPS.h"
#include "ReactionModelDD.h"
#include "ReactionModelDT.h"
#include "MagneticFieldUniform.h"
#include "Visualizer.h"
#include <iostream>
#include <string>
#include <random>
#include <cmath>

using namespace fusion;


int CLI::run(const int argc, char* argv[])
{
    std::cout << "FusionSim - Fusionsreaktor-Simulation" << std::endl;
    if (argc < 2 || std::string(argv[1]) == "--help")
    {
        std::cout << "Optionen:\n"
                  << "  --dd      Deuterium-Deuterium-Fusion\n"
                  << "  --dt      Deuterium-Tritium-Fusion\n"
                  << "  --tmax <t>  Simulationszeit [s]\n"
                  << "  --timestep <dt>   Zeitschritt [s]\n"
                  << "  --particles <n>  Anzahl Teilchen\n";
        return 0;
    }

    double tmax = 1.0;
    double timestep = 0.01;
    int n_particles = 100;
    std::string mode = "dd";

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--dd")
        {
            mode = "dd";
        }
        if (arg == "--dt")
        {
            mode = "dt";
        }
        if (arg == "--tmax" && i + 1 < argc)
        {
            tmax = std::stod(argv[++i]);
        }
        if (arg == "--timestep" && i + 1 < argc)
        {
            timestep = std::stod(argv[++i]);
        }
        if (arg == "--particles" && i + 1 < argc)
        {
            n_particles = std::stoi(argv[++i]);
        }
    }

    if (timestep <= 0.0)
    {
        std::cerr << "Warnung: Zeitschritt muss > 0 sein!" << std::endl;
        return 1;
    }

    if (tmax <= 0.0)
    {
        std::cerr << "Warnung: Simulationszeit muss > 0 sein!" << std::endl;
        return 1;
    }

    if (n_particles < 2)
    {
        std::cerr << "Warnung: Mindestens 2 Teilchen nötig für Fusion!" << std::endl;
        return 1;
    }

    SimulationManager sim;

    auto fieldModel = std::make_shared<FieldModelPotentialMap>(1000.0);
    sim.setFieldModel(fieldModel);

    auto magFieldModel = std::make_shared<MagneticFieldUniform>(Vector3d(0, 0, 1));
    sim.setMagneticFieldModel(magFieldModel);

    if (mode == "dd")
    {
        sim.setReactionModel(std::make_unique<ReactionModelDD>());
    }
    else
    {
        sim.setReactionModel(std::make_unique<ReactionModelDT>());
    }

    constexpr double deuteriumMass = 3.343583719e-27;
    constexpr double deuteriumCharge = 1.60218e-19;
    constexpr double kB = 1.380649e-23;
    constexpr double T = 1e7;

    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<double> vdist(0, std::sqrt(kB * T / deuteriumMass));
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    for (int i = 0; i < n_particles; ++i)
    {
        const double r = 0.01 * uniform(rng);
        const double theta = 2 * M_PI * uniform(rng);
        const double phi = std::acos(2 * uniform(rng) - 1);
        Vector3d pos(
            r * std::sin(phi) * std::cos(theta),
            r * std::sin(phi) * std::sin(theta),
            r * std::cos(phi));
        Vector3d vel(vdist(rng), vdist(rng), vdist(rng));
        sim.addParticle(std::make_unique<ParticleModelSFPS>(
            pos, vel, deuteriumMass, deuteriumCharge, fieldModel, magFieldModel));
    }

    sim.run(tmax, timestep);
    Visualizer::plot(sim.getParticles());
    std::cout << "Simulation abgeschlossen. Ergebnisse in fusion_particles.csv." << std::endl;
    return 0;
}
