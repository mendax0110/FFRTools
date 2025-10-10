
#include "CLI.h"
#include "SimulationManager.h"
#include "FieldModelPotentialMap.h"
#include "ParticleModelSFPS.h"
#include "ReactionModelDD.h"
#include "ReactionModelDT.h"
#include <iostream>
#include <string>

int CLI::run(int argc, char* argv[])
{
    std::cout << "FusionSim - Fusionsreaktor-Simulation" << std::endl;
    if (argc < 2 || std::string(argv[1]) == "--help")
    {
        std::cout << "Optionen:\n"
                  << "  --dd      Deuterium-Deuterium-Fusion\n"
                  << "  --dt      Deuterium-Tritium-Fusion\n"
                  << "  --tmax <t>  Simulationszeit [s]\n"
                  << "  --dt <dt>   Zeitschritt [s]\n"
                  << "  --particles <n>  Anzahl Teilchen\n";
        return 0;
    }

    double tmax = 1.0;
    double dt = 0.01;
    int n_particles = 100;
    std::string mode = "dd";
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--dd") mode = "dd";
        if (arg == "--dt") mode = "dt";
        if (arg == "--tmax" && i+1 < argc) tmax = std::stod(argv[++i]);
        if (arg == "--dt" && i+1 < argc) dt = std::stod(argv[++i]);
        if (arg == "--particles" && i+1 < argc) n_particles = std::stoi(argv[++i]);
    }
    

    if (dt <= 0.0)
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
    Potential pot;
    sim.setFieldModel(std::make_unique<FieldModelPotentialMap>(pot));
    MagneticFieldUniform magfield(Vector3d(0,0,1));

    if (mode == "dd")
        sim.setReactionModel(std::make_unique<ReactionModelDD>());
    else
        sim.setReactionModel(std::make_unique<ReactionModelDT>());
    
    double deuterium_mass = 3.343583719e-27; // kg
    double deuterium_charge = 1.60218e-19; // C
    double kB = 1.380649e-23; // Boltzmann-Konstante
    double T = 1e7; // Temperatur in K (anpassbar)
    std::mt19937 rng(42);
    std::normal_distribution<double> vdist(0, sqrt(kB*T/deuterium_mass));
    for (int i = 0; i < n_particles; ++i)
    {
        double r = 0.01 * ((double)rng()/rng.max()); // 0-1 cm
        double theta = 2*M_PI * ((double)rng()/rng.max());
        double phi = acos(2*((double)rng()/rng.max())-1);
        Vector3d pos(r*sin(phi)*cos(theta), r*sin(phi)*sin(theta), r*cos(phi));
        Vector3d vel(vdist(rng), vdist(rng), vdist(rng));
        sim.addParticle(std::make_unique<ParticleModelSFPS>(pos, vel, deuterium_mass, deuterium_charge, sim.getFieldModel(), &magfield));
    }
    sim.run(tmax, dt);
    Visualizer::plot(sim.getParticles());
    std::cout << "Simulation abgeschlossen. Ergebnisse in fusion_particles.csv." << std::endl;
    return 0;
}
