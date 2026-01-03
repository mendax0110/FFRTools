#include "CLI.h"
#include "SimulationManager.h"
#include "FieldModelPotentialMap.h"
#include "FarnsworthFusorFieldModel.h"
#include "ParticleModelSFPS.h"
#include "ReactionModelDD.h"
#include "ReactionModelDT.h"
#include "MagneticFieldUniform.h"
#include "Visualizer.h"
#include "PhysicalConstants.h"
#include <iostream>
#include <string>
#include <random>
#include <cmath>

using namespace fusion;


int CLI::run(const int argc, char* argv[])
{
    std::cout << "FusionSim - Fusion Reactor Simulation" << std::endl;
    if (argc < 2 || std::string(argv[1]) == "--help")
    {
        std::cout << "Options:\n"
                  << "  --dd             Deuterium-Deuterium fusion\n"
                  << "  --dt             Deuterium-Tritium fusion\n"
                  << "  --fusor          Farnsworth Fusor mode (IEC)\n"
                  << "  --tmax <t>       Simulation time [s] (default: 1e-6)\n"
                  << "  --timestep <dt>  Time step [s] (default: 1e-10)\n"
                  << "  --particles <n>  Number of particles (default: 100)\n"
                  << "  --temperature <T> Ion temperature [K] (default: 1e4)\n"
                  << "  --voltage <V>    Cathode voltage [V] for fusor (default: -30000)\n"
                  << "  --pressure <P>   Chamber pressure [mbar] (default: 0.2)\n"
                  << "  --threads <n>    Number of CPU threads (default: all available)\n"
                  << "  --thermal       Enable thermal dynamics model\n";
        return 0;
        // ./FusionSim --fusor --dd --particles 1000 --tmax 1e-6 --timestep 1e-11 --voltage -30000 --pressure 0.023 --temperature 10000
    }

    double tmax = 1.0e-6;
    double timestep = 1.0e-10;
    int n_particles = 100;
    double temperature = 1.0e4;
    double cathodeVoltage = -30000.0;
    double pressure_mbar = 0.2;
    int numThreads = 0;
    std::string mode = "dd";
    bool fusorMode = false;
    bool enableThermalDynamics = false;

    for (int i = 1; i < argc; ++i)
    {
        const std::string arg = argv[i];
        if (arg == "--dd")
        {
            mode = "dd";
        }
        else if (arg == "--dt")
        {
            mode = "dt";
        }
        else if (arg == "--fusor")
        {
            fusorMode = true;
        }
        else if (arg == "--tmax" && i + 1 < argc)
        {
            tmax = std::stod(argv[++i]);
        }
        else if (arg == "--timestep" && i + 1 < argc)
        {
            timestep = std::stod(argv[++i]);
        }
        else if (arg == "--particles" && i + 1 < argc)
        {
            n_particles = std::stoi(argv[++i]);
        }
        else if (arg == "--temperature" && i + 1 < argc)
        {
            temperature = std::stod(argv[++i]);
        }
        else if (arg == "--voltage" && i + 1 < argc)
        {
            cathodeVoltage = std::stod(argv[++i]);
        }
        else if (arg == "--pressure" && i + 1 < argc)
        {
            pressure_mbar = std::stod(argv[++i]);
        }
        else if (arg == "--threads" && i + 1 < argc)
        {
            numThreads = std::stoi(argv[++i]);
        }
        else if (arg == "--thermal")
        {
            enableThermalDynamics = true;
        }
    }

    if (timestep <= 0.0)
    {
        std::cerr << "Error: Time step must be > 0!" << std::endl;
        return 1;
    }

    if (tmax <= 0.0)
    {
        std::cerr << "Error: Simulation time must be > 0!" << std::endl;
        return 1;
    }

    if (n_particles < 2)
    {
        std::cerr << "Error: At least 2 particles required for fusion!" << std::endl;
        return 1;
    }

    SimulationManager sim;

    if (numThreads > 0)
    {
        sim.setNumThreads(numThreads);
    }

    if (enableThermalDynamics)
    {
        sim.enableThermalDynamics(true);
        std::cout << "Thermal dynamics model enabled.\n";
    }
    else
    {
        std::cerr << "Thermal dynamics model disabled.\n";
    }

    std::shared_ptr<IFieldModel> fieldModel;
    std::shared_ptr<FarnsworthFusorFieldModel> fusorField = nullptr;

    if (fusorMode)
    {
        constexpr double innerGridRadius = 0.008;
        constexpr double outerGridRadius = 0.04;
        constexpr double gridTransparency = 0.95;
        constexpr double wireDiameter = 0.001;
        constexpr int innerWireCount = 12;
        constexpr int outerWireCount = 16;

        fusorField = std::make_shared<FarnsworthFusorFieldModel>(
            innerGridRadius,
            outerGridRadius,
            cathodeVoltage,
            gridTransparency,
            wireDiameter,
            innerWireCount,
            outerWireCount,
            GridType::ROSENSTIEHL_SPHERICAL);
        fieldModel = fusorField;

        double pressure_Pa_local = pressure_mbar * 100.0;
        fusorField->setOperatingPressure(pressure_Pa_local);
        fusorField->setGridTemperature(293.15);
        fusorField->setChamberTemperature(293.15);

        std::cout << "\n=== Farnsworth Fusor Configuration ===" << std::endl;
        std::cout << "Grid Geometry:" << std::endl;
        std::cout << "  Inner grid (cathode) radius: " << innerGridRadius * 100.0 << " cm" << std::endl;
        std::cout << "  Outer grid (anode) radius: " << outerGridRadius * 100.0 << " cm" << std::endl;
        std::cout << "  Wire diameter: " << wireDiameter * 1000.0 << " mm" << std::endl;
        std::cout << "  Inner grid wire count: " << innerWireCount << std::endl;
        std::cout << "  Outer grid wire count: " << outerWireCount << std::endl;
        std::cout << "  Grid type: Rosenstiehl Spherical" << std::endl;
        std::cout << "  Nominal transparency: " << gridTransparency * 100.0 << " %" << std::endl;
        std::cout << "  Effective transparency: " << fusorField->calculateEffectiveTransparency() * 100.0 << " %" << std::endl;

        std::cout << "\nElectrical Parameters:" << std::endl;
        std::cout << "  Cathode voltage: " << cathodeVoltage / 1000.0 << " kV" << std::endl;
        std::cout << "  Resonant frequency: " << fusorField->getResonantFrequency() / 1000.0 << " kHz" << std::endl;
        std::cout << "  Peak-to-peak current: " << fusorField->getPeakToPeakCurrent() << " A" << std::endl;

        std::cout << "\nIon Dynamics:" << std::endl;
        double maxEnergy_eV = fusorField->getMaxIonEnergy() / constants::eCharge;
        double maxSpeed = fusorField->getMaxIonSpeed();
        std::cout << "  Max ion energy: " << maxEnergy_eV / 1000.0 << " keV" << std::endl;
        std::cout << "  Max ion speed: " << maxSpeed / 1000.0 << " km/s" << std::endl;
        double oscillationPeriod = fusorField->getOscillationPeriod();
        std::cout << "  Ion oscillation period: " << oscillationPeriod * 1.0e9 << " ns" << std::endl;
        double oscillationFreq = 1.0 / oscillationPeriod;
        std::cout << "  Ion oscillation frequency: " << oscillationFreq / 1000.0 << " kHz" << std::endl;

        std::cout << "\nResonant Circuit:" << std::endl;
        double t_sample = 0.0;
        double current_t0 = fusorField->calculateResonantCurrent(t_sample);
        double current_t1 = fusorField->calculateResonantCurrent(0.25 / fusorField->getResonantFrequency());
        std::cout << "  Current at t=0: " << current_t0 << " A" << std::endl;
        std::cout << "  Current at T/4: " << current_t1 << " A" << std::endl;

        std::cout << "\nVacuum and Breakdown:" << std::endl;
        double gap = outerGridRadius - innerGridRadius;
        double V_paschen = FarnsworthFusorFieldModel::calculatePaschenBreakdown(pressure_Pa_local, gap);
        std::cout << "  Electrode gap: " << gap * 100.0 << " cm" << std::endl;
        std::cout << "  Paschen breakdown voltage: " << V_paschen / 1000.0 << " kV" << std::endl;

        std::cout << "\nCollision Physics:" << std::endl;
        double meanFreePath = FarnsworthFusorFieldModel::calculateMeanFreePath(pressure_Pa_local, temperature);
        std::cout << "  Mean free path: " << meanFreePath * 1000.0 << " mm" << std::endl;
        double ionCrossSection = FarnsworthFusorFieldModel::calculateIonizationCrossSection(maxEnergy_eV);
        std::cout << "  Ionization cross-section at max energy: " << ionCrossSection / 1.0e-20 << " × 10⁻²⁰ m²" << std::endl;

        std::cout << "\nOperating Mode:" << std::endl;
        double testCurrent = 0.05;
        OperatingMode opMode = fusorField->determineOperatingMode(testCurrent);
        std::cout << "  Pressure: " << pressure_mbar << " mbar" << std::endl;
        std::cout << "  Current: " << testCurrent << " A" << std::endl;
        std::cout << "  Mode: ";
        switch (opMode)
        {
            case OperatingMode::VACUUM_PUMPING:
                std::cout << "VACUUM_PUMPING" << std::endl;
                break;
            case OperatingMode::PLASMA_IGNITION:
                std::cout << "PLASMA_IGNITION" << std::endl;
                break;
            case OperatingMode::STABLE_OPERATION:
                std::cout << "STABLE_OPERATION" << std::endl;
                break;
            case OperatingMode::STAR_MODE:
                std::cout << "STAR_MODE" << std::endl;
                break;
        }

        std::cout << "\nThermal Status:" << std::endl;
        std::cout << "  Grid temperature: " << fusorField->getGridTemperature() << " K" << std::endl;
        std::cout << "  Chamber temperature: " << fusorField->getChamberTemperature() << " K" << std::endl;
        std::cout << "  Grid temp safe: " << (fusorField->isGridTemperatureSafe() ? "Yes" : "No") << std::endl;
        std::cout << "  Chamber temp safe: " << (fusorField->isChamberTemperatureSafe() ? "Yes" : "No") << std::endl;
        std::cout << "===================================\n" << std::endl;
    }
    else
    {
        fieldModel = std::make_shared<FieldModelPotentialMap>(1000.0);
    }
    sim.setFieldModel(fieldModel);

    auto magFieldModel = std::make_shared<MagneticFieldUniform>(Vector3d(0.0, 0.0, 0.0));
    sim.setMagneticFieldModel(magFieldModel);

    double pressure_Pa = pressure_mbar * 100.0;
    double particleDensity = pressure_Pa / (constants::kBoltzmann * temperature);
    sim.setParticleDensity(particleDensity);

    std::cout << "Chamber pressure: " << pressure_mbar << " mbar (" << pressure_Pa << " Pa)" << std::endl;
    std::cout << "Particle density: " << particleDensity << " m^-3" << std::endl;

    if (!fusorMode)
    {
        double meanFreePathCalc = FarnsworthFusorFieldModel::calculateMeanFreePath(pressure_Pa, temperature);
        std::cout << "Mean free path: " << meanFreePathCalc * 1000.0 << " mm" << std::endl;
    }

    if (fusorMode && fusorField)
    {
        double debyeLength = FarnsworthFusorFieldModel::calculateDebyeLength(temperature, particleDensity);
        double plasmaFrequency = FarnsworthFusorFieldModel::calculatePlasmaFrequency(particleDensity);
        std::cout << "Debye length: " << debyeLength * 1.0e6 << " um" << std::endl;
        std::cout << "Plasma frequency: " << plasmaFrequency / (2.0 * constants::pi * 1.0e6) << " MHz" << std::endl;
    }

    if (mode == "dd")
    {
        sim.setReactionModel(std::make_unique<ReactionModelDD>());
        std::cout << "Reaction: Deuterium-Deuterium" << std::endl;
    }
    else
    {
        sim.setReactionModel(std::make_unique<ReactionModelDT>());
        std::cout << "Reaction: Deuterium-Tritium" << std::endl;
    }

    const double thermalSpeed = std::sqrt(constants::kBoltzmann * temperature / constants::massDeuterium);

    std::cout << "Ion temperature: " << temperature << " K" << std::endl;
    std::cout << "Thermal speed: " << thermalSpeed << " m/s" << std::endl;
    std::cout << "Number of particles: " << n_particles << std::endl;

    std::mt19937 rng(std::random_device{}());
    std::normal_distribution<double> vdist(0.0, thermalSpeed);
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    double spawnRadius = 0.10;
    double innerRadius = 0.0;
    if (fusorMode)
    {
        spawnRadius = 0.075;
        innerRadius = 0.065;
    }

    for (int i = 0; i < n_particles; ++i)
    {
        double r;
        if (fusorMode)
        {
            r = innerRadius + (spawnRadius - innerRadius) * uniform(rng);
        }
        else
        {
            r = spawnRadius * std::cbrt(uniform(rng));
        }

        const double theta = 2.0 * constants::pi * uniform(rng);
        const double phi = std::acos(2.0 * uniform(rng) - 1.0);

        const Vector3d pos(
            r * std::sin(phi) * std::cos(theta),
            r * std::sin(phi) * std::sin(theta),
            r * std::cos(phi));

        Vector3d vel;
        if (fusorMode)
        {
            const double posNorm = pos.norm();
            if (posNorm > 1e-12)
            {
                const Vector3d radialUnit = pos / posNorm;
                double inwardSpeed = thermalSpeed * (0.5 + uniform(rng));
                vel = -inwardSpeed * radialUnit;

                double tangentialSpeed = thermalSpeed * 0.1 * (uniform(rng) - 0.5);
                Vector3d perpAxis(0.0, 0.0, 1.0);
                if (std::abs(radialUnit.z) > 0.9)
                {
                    perpAxis = Vector3d(1.0, 0.0, 0.0);
                }
                Vector3d tangent = radialUnit.cross(perpAxis).normalized();
                vel = vel + tangentialSpeed * tangent;
            }
            else
            {
                vel = Vector3d(vdist(rng), vdist(rng), vdist(rng));
            }
        }
        else
        {
            vel = Vector3d(vdist(rng), vdist(rng), vdist(rng));
        }

        sim.addParticle(std::make_unique<ParticleModelSFPS>(pos, vel, constants::massDeuterium, constants::eCharge, fieldModel, magFieldModel));
    }

    std::cout << "Running simulation for " << tmax << " s with dt = " << timestep << " s" << std::endl;
    sim.run(tmax, timestep);

    Visualizer::plot(sim.getParticles());
    std::cout << "Simulation complete. Results saved to fusion_particles.csv." << std::endl;
    std::cout << "Final particle count: " << sim.getParticles().size() << std::endl;

    return 0;
}
