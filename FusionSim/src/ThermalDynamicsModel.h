#pragma once

#include "PhysicalConstants.h"
#include <cmath>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Thermal Dynamics Model for grid and chamber. \class ThermalDynamicsModel
    class ThermalDynamicsModel
    {
    public:

        /// @brief Gridmaterials \enum GridMaterial
        enum class GridMaterial
        {
            TUNGSTEN
        };

        /**
         * @brief Constructor for ThermalDynamicsModel.
         * @param gridMass Mass of the grid in kg.
         * @param chamberMass Mass of the chamber in kg.
         * @param gridSurfaceArea Surface area of the grid in m^2.
         * @param chamberSurfaceArea Surface area of the chamber in m^2.
         * @param material Material of the grid.
         */
        explicit ThermalDynamicsModel(
            const double gridMass = 0.05,
            const double chamberMass = 2.0,
            const double gridSurfaceArea = 0.003,
            const double chamberSurfaceArea = 0.5,
            const GridMaterial material = GridMaterial::TUNGSTEN)
                : m_gridMass(gridMass)
                , m_chamberMass(chamberMass)
                , m_gridSurfaceArea(gridSurfaceArea)
                , m_chamberSurfaceArea(chamberSurfaceArea)
                , m_gridTemperature(0.00)
                , m_chamberTemperature(28.00)
                , m_ambientTemperature(25.00)
                , m_gridMaterial(material)
        {

        }

        void updateTemperatures(const double dt, const double current_A,
                                const double particleFlux, const double avgParticleEnergy,
                                const double operatingPressure)
        {
            // Update temp....
            const double gridHeating = calculateGridHeating(current_A, particleFlux, avgParticleEnergy);
            const double gridCooling = calculateGridCooling(operatingPressure);
            const double dT_grid = (gridHeating - gridCooling) * dt / (m_gridMass * m_gridSpecificHeat);
            m_gridTemperature += dT_grid;

            // heat transfer from grid to chamber, only outer grid facing...
            const double gridToChamberHeat = calculateRadiativeHeatTransfer(m_gridTemperature, m_chamberTemperature, m_gridSurfaceArea * 0.5);

            // chamber temp update...
            const double chamberHeating = gridToChamberHeat;
            const double chamberCooling = calculateChamberCooling(operatingPressure);
            const double dT_chamber = (chamberHeating - chamberCooling) * dt / (m_chamberMass * m_chamberSpecificHeat);
            m_chamberTemperature += dT_chamber;

            // prevent non physical temps...
            m_gridTemperature = std::max(m_gridTemperature, m_ambientTemperature);
            m_chamberTemperature = std::max(m_chamberTemperature, m_ambientTemperature);
        }

        /**
         * @brief Getter for grid temperature.
         * @return Grid temperature in Kelvin.
         */
        [[nodiscard]] double getGridTemperature() const { return m_gridTemperature; }

        /**
         * @brief Getter for chamber temperature.
         * @return Chamber temperature in Kelvin.
         */
        [[nodiscard]] double getChamberTemperature() const { return m_chamberTemperature; }

        /**
         * @brief Calculate the electrical resistance of the grid.
         * @return Grid resistance in ohms.
         */
        [[nodiscard]] double getGridResistance() const
        {
            constexpr  double T0 = 293.15;
            const double rho_T = m_resistivity * (1.0 + m_tempCoeffResistance * (m_gridTemperature - T0));

            // cylindrical high voltage wire
            const double wireLength = estimateWireLength();
            constexpr  double wireRadius = 0.0005; // 0.5 mm radius
            constexpr  double wireArea = constants::pi * wireRadius * wireRadius;
            return rho_T * wireLength / wireArea;
        }

        /**
         * @brief Calculate the thermal time constant of the grid.
         * @return Grid thermal time constant in seconds.
         */
        [[nodiscard]] double getGridThermalTimeConstant() const
        {
            // T = (m * c_p) / (h * A)
            constexpr double h_effective = 100.0; // W/m^2*K
            return (m_gridMass * m_gridSpecificHeat) / (h_effective * m_gridSurfaceArea);
        }

        /**
         * @brief Setter for grid temperature.
         * @param temp_K Temperature in Kelvin.
         */
        void setGridTemperature(const double temp_K) { m_gridTemperature = temp_K; }

        /**
         * @brief Setter for chamber temperature.
         * @param temp_K Temperature in Kelvin.
         */
        void setChamberTemperature(const double temp_K) { m_chamberTemperature = temp_K; }

        /**
         * @brief Setter for ambient temperature.
         * @param temp_K Temperature in Kelvin.
         */
        void setAmbientTemperature(const double temp_K) { m_ambientTemperature = temp_K; }


    private:

        /**
         * @brief Calculate the heating power of the grid.
         * @param current_A Current through the grid in amperes.
         * @param particleFlux Particle flux hitting the grid in particles per second.
         * @param avgParticleEnergy Average energy of particles in joules.
         * @return Heating power in watts.
         */
        [[nodiscard]] double calculateGridHeating(const double current_A, const double particleFlux, const double avgParticleEnergy) const
        {
            const double R = getGridResistance();
            const double ohmicPower = current_A * current_A * R; // P=I^2*R

            constexpr double gridTransparency = 0.95; // assuming franction of particles TODO: CHECK THIS AGAIN ADRGOS
            const double particlesHitting = particleFlux * (1.0 - gridTransparency);
            const double bombardmentPower = particlesHitting * avgParticleEnergy;
            return ohmicPower + bombardmentPower;
        }

        /**
         * @brief Calculate the convective cooling power.
         * @param T_surface Surface temperature in Kelvin.
         * @param T_gas Gas temperature in Kelvin.
         * @param area Surface area in m^2.
         * @param pressure Operating pressure in pascals.
         * @return Convective cooling power in watts.
         */
        static double calculateConvectiveCooling(const double T_surface, const double T_gas, const double area, const double pressure)
        {
            // if the we have very low pressure, the convective effect is negligible,
            // therefore just a simple pressure-predendent mode
            constexpr double P_ref = 101325.0;
            constexpr double h_ref = 10.0; // W/m^2*K at P
            const double h = h_ref * (pressure / P_ref);
            return h * area * (T_surface - T_gas);
        }

        /**
         * @brief Estimate the total wire length of the grid.
         * @return The estimated wire length in meters.
         */
        static double estimateWireLength()
        {
            constexpr double innerGridRadius = 0.04;
            constexpr int numWires = 4;
            return numWires * 2.0 * constants::pi * innerGridRadius;
        }

        /**
         * @brief Calculate the cooling power of the grid.
         * @param operatingPressure Operating pressure in pascals.
         * @return Cooling power in watts.
         */
        [[nodiscard]] double calculateGridCooling(const double operatingPressure) const
        {
            // P = E * sigma * A * (T^4 - T_ambient^4)
            const double radiativeCooling = constants::stefanBoltzmann * m_gridSurfaceArea * (std::pow(m_gridTemperature, 4) - std::pow(m_ambientTemperature, 4));

            constexpr double suportThermalConductance = 0.1; // W/K
            const double conductiveCooling = suportThermalConductance * (m_gridTemperature - m_chamberTemperature);

            const double convectiveCooling = calculateConvectiveCooling(m_gridTemperature, m_chamberTemperature, m_gridSurfaceArea, operatingPressure);

            return radiativeCooling + conductiveCooling + convectiveCooling;
        }

        /**
         * @brief Calculate the cooling power of the chamber.
         * @param operatingPressure Operating pressure in pascals.
         * @return Cooling power in watts.
         */
        [[nodiscard]] double calculateChamberCooling(const double operatingPressure) const
        {
            constexpr double h_external = 20.0; // W/m^2*K
            const double externalCooling = h_external * m_chamberSurfaceArea * (m_chamberTemperature - m_ambientTemperature);
            return externalCooling;
        }

        /**
         * @brief Calculate the radiative heat transfer between two surfaces.
         * @param T_hot Temperature of the hot surface in Kelvin.
         * @param T_cold Temperature of the cold surface in Kelvin.
         * @param area Surface area in m^2.
         * @return Radiative heat transfer in watts.
         */
        [[nodiscard]] double calculateRadiativeHeatTransfer(const double T_hot, const double T_cold, const double area) const
        {
            return m_emissivity * constants::stefanBoltzmann * area * (std::pow(T_hot, 4) - std::pow(T_cold, 4));
        }

        /**
         * @brief Update material properties based on selected grid material.
         */
        void updateMaterialProperties()
        {
            switch (m_gridMaterial)
            {
                case GridMaterial::TUNGSTEN:
                    m_gridSpecificHeat = 132.0; // J/(kg·K)
                    m_gridThermalConductivity = 173.0; // W/(m·K)
                    m_resistivity = 5.6e-8; // ohm·m
                    m_tempCoeffResistance = 0.001; // 1/K
                    m_emissivity = 0.3; // dimensionless
                    m_chamberSpecificHeat = 500.0;
                    break;
                default:
                    break;
            }
        }


        /// @brief Physical dimensions
        double m_gridMass;
        double m_chamberMass;
        double m_gridSurfaceArea;
        double m_chamberSurfaceArea;

        /// @brief Thermal state
        double m_gridTemperature;
        double m_chamberTemperature;
        double m_ambientTemperature;

        /// @brief Material properties
        GridMaterial m_gridMaterial;
        double m_gridSpecificHeat{};
        double m_gridThermalConductivity{};
        double m_resistivity{};
        double m_tempCoeffResistance{};
        double m_emissivity{};
        double m_chamberSpecificHeat{};
    };
}