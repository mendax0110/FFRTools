#include "CLI.h"
#include "SimulationManager.h"
#include "FieldModelPotentialMap.h"
#include "ParticleModelSFPS.h"
#include "ReactionModelDD.h"
#include "ReactionModelDT.h"
#include "MagneticFieldUniform.h"
#include "DummyFieldModel.h"
#include "Visualizer.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <random>
#include <cmath>

int CLI::run(int argc, char* argv[])
{
    std::cout << "FusionSim - Fusionsreaktor-Simulation gestartet\n";

    if (argc < 2 || std::string(argv[1]) == "--help")
    {
        std::cout << "\nVerwendung: FusionSim [Optionen]\n\n"
                  << "Optionen:\n"
                  << "  --dd              Deuterium-Deuterium-Fusion\n"
                  << "  --dt              Deuterium-Tritium-Fusion\n"
                  << "  --tmax <t>        Simulationszeit [s]\n"
                  << "  --dt <dt>         Zeitschritt [s]\n"
                  << "  --particles <n>   Anzahl Teilchen\n\n"
                  << "Beispiel: ./FusionSim --dt 1e-4 --tmax 0.1 --particles 500 --dt\n";
        return 0;
    }

    double tmax = 1.0;
    double dt = 0.01;
    int n_particles = 100;
    std::string mode = "dd";

    try
    {
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "--dd") mode = "dd";
            else if (arg == "--dt") mode = "dt";
            else if (arg == "--tmax" && i + 1 < argc)
                tmax = std::stod(argv[++i]);
            else if (arg == "--dt" && i + 1 < argc)
                dt = std::stod(argv[++i]);
            else if (arg == "--particles" && i + 1 < argc)
                n_particles = std::stoi(argv[++i]);
            else
                std::cerr << "Unbekannte Option: " << arg << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fehler beim Parsen der Eingabeparameter: " << e.what() << std::endl;
        return 1;
    }

    // --- Validierung ---
    if (dt <= 0.0)
    {
        std::cerr << "Fehler: Zeitschritt (--dt) muss > 0 sein.\n";
        return 1;
    }

    if (tmax <= 0.0)
    {
        std::cerr << "Fehler: Simulationszeit (--tmax) muss > 0 sein.\n";
        return 1;
    }

    if (n_particles < 2)
    {
        std::cerr << "Fehler: Mindestens 2 Teilchen nötig für Fusion!\n";
        return 1;
    }

    std::cout << "\nSimulation wird vorbereitet...\n"
              << "  Modus: " << (mode == "dd" ? "Deuterium-Deuterium" : "Deuterium-Tritium") << "\n"
              << "  Teilchenzahl: " << n_particles << "\n"
              << "  Zeitschritt:  " << dt << " s\n"
              << "  Simulationszeit: " << tmax << " s\n";

    try
    {
        SimulationManager sim;
        Potential pot;
        // ./FusionSim --tmax 0.01 --dt 0.00001 --particles 100 --dd
        sim.setFieldModel(std::make_unique<FieldModelPotentialMap>(pot));
        MagneticFieldUniform magfield(Vector3d<Velocity>(Velocity(0), Velocity(0), Velocity(1)));

        auto testField = sim.getFieldModel()->getFieldAt(Vector3d<Position>::fromDouble(0.0, 0.0, 0.0));
        if (std::isnan(testField.x.value) || std::isnan(testField.y.value) || std::isnan(testField.z.value))
        {
            std::cerr << "[DEBUG] Warnung: Das elektrische Feld liefert NaN an (0,0,0)!\n";
            std::cerr << "[DEBUG] Prüfe die Initialisierung des Potentials und der Map!\n";
        }
        else
        {
            std::cout << "[DEBUG] Testfeld an (0,0,0): (" << testField.x << ", " << testField.y << ", " << testField.z << ")\n";
        }

        if (mode == "dd")
            sim.setReactionModel(std::make_unique<ReactionModelDD>());
        else
            sim.setReactionModel(std::make_unique<ReactionModelDT>());

        Mass deuterium_mass = Mass(3.343583719e-27); // kg
        Charge deuterium_charge = Charge(1.60218e-19);  // C
        double kB = 1.380649e-23;               // Boltzmann-Konstante
        double T = 1e7;                         // Temperatur in K (anpassbar)

        std::mt19937 rng(42);
        std::normal_distribution<double> vdist(0.0, std::sqrt(kB * T / deuterium_mass.value));
        std::uniform_real_distribution<double> udist(0.0, 1.0);

        std::cout << "Erzeuge " << n_particles << " Teilchen...\n";

        for (int i = 0; i < n_particles; ++i)
        {
            double r = 0.01 * udist(rng); // 0-1 cm
            double theta = 2 * M_PI * udist(rng);
            double phi = std::acos(2 * udist(rng) - 1);

            Vector3d<Position> pos(
                Position(r * std::sin(phi) * std::cos(theta)),
                Position(r * std::sin(phi) * std::sin(theta)),
                Position(r * std::cos(phi)));

            Vector3d<Velocity> vel(
                Velocity(vdist(rng)),
                Velocity(vdist(rng)),
                Velocity(vdist(rng)));

            sim.addParticle(std::make_unique<ParticleModelSFPS>(
                pos, vel, deuterium_mass, deuterium_charge,
                sim.getFieldModel(), static_cast<const IMagneticFieldModel*>(&magfield)));

            if ((i + 1) % (n_particles / 10 == 0 ? 1 : n_particles / 10) == 0)
                std::cout << "  → " << (100 * (i + 1) / n_particles) << "% erstellt\n";
        }

        std::cout << "Starte Simulation...\n";
        sim.run(tmax, dt);

        std::cout << "Simulation abgeschlossen. Visualisiere Ergebnisse...\n";
        Visualizer::plot(sim.getParticles());

        std::cout << "Simulation erfolgreich abgeschlossen!\n"
                  << "   Ergebnisse in 'fusion_particles.csv'.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Laufzeitfehler während der Simulation: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unbekannter Fehler aufgetreten!\n";
        return 1;
    }

    return 0;
}
