#pragma once
#include "IFieldModel.h"
#include "PhysicalConstants.h"
#include <cmath>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Operating modes for the Farnsworth Fusor. \enum OperatingMode
    enum class OperatingMode
    {
        VACUUM_PUMPING,
        PLASMA_IGNITION,
        STABLE_OPERATION,
        STAR_MODE
    };

    /// @brief Grid types for the Farnsworth Fusor. \enum GridType
    enum class GridType
    {
        ROSENSTIEHL_SPHERICAL,
        CYLINDRICAL_WIRE,
        MESH_GRID
    };

    /// @brief Farnsworth Fusor Field Model. \class FarnsworthFusorFieldModel
    class FarnsworthFusorFieldModel : public IFieldModel
    {
    public:
        /// @brief Default inner grid radius in meters.
        static constexpr double defaultInnerGridRadius = 0.008;

        /// @brief Default outer grid radius in meters.
        static constexpr double defaultOuterGridRadius = 0.04;

        /// @brief Default cathode voltage in volts.
        static constexpr double defaultCathodeVoltage = -30000.0;

        /// @brief Default grid transparency (0.0 to 1.0).
        static constexpr double defaultGridTransparency = 0.95;

        /// @brief Default wire diameter for grid construction in meters.
        static constexpr double defaultWireDiameter = 0.001;

        /// @brief Default number of wires in the inner grid.
        static constexpr int defaultInnerGridWireCount = 12;

        /// @brief Default number of wires in the outer grid.
        static constexpr int defaultOuterGridWireCount = 16;

        /// @brief Default resonant frequency of the power supply in Hz.
        static constexpr double defaultResonantFrequency = 35000.0;

        /// @brief Default peak-to-peak current in amperes.
        static constexpr double defaultPeakToPeakCurrent = 80.0;

        /// @brief Default operating pressure in pascals.
        static constexpr double defaultOperatingPressure = 1.0;

        /// @brief Default grid temperature limit in Kelvin.
        static constexpr double defaultGridTempLimit = 1273.15;

        /// @brief Default chamber temperature limit in Kelvin.
        static constexpr double defaultChamberTempLimit = 373.15;

        /**
         * @brief Constructor for FarnsworthFusorFieldModel with default parameters.
         */
        FarnsworthFusorFieldModel()
            : m_innerGridRadius(defaultInnerGridRadius)
            , m_outerGridRadius(defaultOuterGridRadius)
            , m_cathodeVoltage(defaultCathodeVoltage)
            , m_gridTransparency(defaultGridTransparency)
            , m_wireDiameter(defaultWireDiameter)
            , m_innerGridWireCount(defaultInnerGridWireCount)
            , m_outerGridWireCount(defaultOuterGridWireCount)
            , m_gridType(GridType::ROSENSTIEHL_SPHERICAL)
            , m_resonantFrequency(defaultResonantFrequency)
            , m_peakToPeakCurrent(defaultPeakToPeakCurrent)
            , m_operatingPressure(defaultOperatingPressure)
            , m_gridTempLimit(defaultGridTempLimit)
            , m_chamberTempLimit(defaultChamberTempLimit)
            , m_currentGridTemp(293.15)
            , m_currentChamberTemp(293.15)
            , m_geometryFactor(calculateGeometryFactor())
        {
        }

        /**
         * @brief Constructor for FarnsworthFusorFieldModel with custom parameters.
         * @param innerGridRadius Inner grid (cathode) radius in meters.
         * @param outerGridRadius Outer grid radius in meters.
         * @param cathodeVoltage Cathode voltage in volts.
         * @param gridTransparency Grid transparency (0.0 to 1.0).
         * @param wireDiameter Wire diameter for grid construction in meters.
         * @param innerGridWireCount Number of wires in the inner grid.
         * @param outerGridWireCount Number of wires in the outer grid.
         * @param gridType Type of grid configuration.
         */
        FarnsworthFusorFieldModel(
            const double innerGridRadius,
            const double outerGridRadius,
            const double cathodeVoltage,
            const double gridTransparency = 0.95,
            const double wireDiameter = defaultWireDiameter,
            const int innerGridWireCount = defaultInnerGridWireCount,
            const int outerGridWireCount = defaultOuterGridWireCount,
            const GridType gridType = GridType::ROSENSTIEHL_SPHERICAL)
            : m_innerGridRadius(innerGridRadius)
            , m_outerGridRadius(outerGridRadius)
            , m_cathodeVoltage(cathodeVoltage)
            , m_gridTransparency(gridTransparency)
            , m_wireDiameter(wireDiameter)
            , m_innerGridWireCount(innerGridWireCount)
            , m_outerGridWireCount(outerGridWireCount)
            , m_gridType(gridType)
            , m_resonantFrequency(defaultResonantFrequency)
            , m_peakToPeakCurrent(defaultPeakToPeakCurrent)
            , m_operatingPressure(defaultOperatingPressure)
            , m_gridTempLimit(defaultGridTempLimit)
            , m_chamberTempLimit(defaultChamberTempLimit)
            , m_currentGridTemp(293.15)
            , m_currentChamberTemp(293.15)
            , m_geometryFactor(calculateGeometryFactor())
        {
        }

        /**
         * @brief Getter for the electric field at a given position.
         * @param position The position where the field is queried.
         * @return The electric field vector at the given position.
         */
        [[nodiscard]] Vector3d getFieldAt(const Vector3d& position) const override
        {
            const double r = position.norm();

            if (r < 1e-12)
            {
                return Vector3d(0.0, 0.0, 0.0);
            }

            const double E_r = calculateRadialField(r);

            const double invR = 1.0 / r;
            return Vector3d(
                E_r * position.x * invR,
                E_r * position.y * invR,
                E_r * position.z * invR);
        }

        /**
         * @brief Getter for the electric potential at a given radius.
         * @param r The radial distance from the center in meters.
         * @return The electric potential at the given radius in volts.
         */
        [[nodiscard]] double getPotentialAt(const double r) const
        {
            if (r <= m_innerGridRadius)
            {
                return m_cathodeVoltage;
            }
            else if (r <= m_outerGridRadius)
            {
                const double invRi = 1.0 / m_innerGridRadius;
                const double invRo = 1.0 / m_outerGridRadius;
                const double invR = 1.0 / r;

                const double phi = m_cathodeVoltage * (invR - invRo) / (invRi - invRo);
                return phi;
            }
            else
            {
                return 0.0;
            }
        }

        /**
         * @brief Calculate the kinetic energy gain of an ion moving from r_start to r_end.
         * @param r_start Starting radial position in meters.
         * @param r_end Ending radial position in meters.
         * @return The kinetic energy gain in joules.
         */
        [[nodiscard]] double getKineticEnergyGain(const double r_start, const double r_end) const
        {
            const double V_start = getPotentialAt(r_start);
            const double V_end = getPotentialAt(r_end);
            return constants::eCharge * (V_start - V_end);
        }

        /**
         * @brief Getter for inner grid radius.
         * @return A double representing the inner grid radius in meters.
         */
        [[nodiscard]] double getInnerGridRadius() const { return m_innerGridRadius; }

        /**
         * @brief Getter for outer grid radius.
         * @return A double representing the outer grid radius in meters.
         */
        [[nodiscard]] double getOuterGridRadius() const { return m_outerGridRadius; }

        /**
         * @brief Getter for cathode voltage.
         * @return A double representing the cathode voltage in volts.
         */
        [[nodiscard]] double getCathodeVoltage() const { return m_cathodeVoltage; }

        /**
         * @brief Getter for grid transparency.
         * @return A double representing the grid transparency (0.0 to 1.0).
         */
        [[nodiscard]] double getGridTransparency() const { return m_gridTransparency; }

        /**
         * @brief Getter for maximum ion energy.
         * @return A double representing the maximum ion energy in joules.
         */
        [[nodiscard]] double getMaxIonEnergy() const
        {
            return constants::eCharge * std::abs(m_cathodeVoltage);
        }

        /**
         * @brief Getter for maximum ion speed.
         * @return A double representing the maximum ion speed in meters per second.
         */
        [[nodiscard]] double getMaxIonSpeed() const
        {
            const double E_max = getMaxIonEnergy();
            return std::sqrt(2.0 * E_max / constants::massDeuterium);
        }

        /**
         * @brief Static method to calculate the Debye length.
         * @param temperature_K Temperature in Kelvin.
         * @param particleDensity Particle density in particles per cubic meter.
         * @return The Debye length in meters.
         */
        static double calculateDebyeLength(const double temperature_K, const double particleDensity)
        {
            const double numerator = constants::epsilon0 * constants::kBoltzmann * temperature_K;
            const double denominator = particleDensity * constants::eCharge * constants::eCharge;
            return std::sqrt(numerator / denominator);
        }

        /**
         * @brief Static method to calculate the plasma frequency.
         * @param particleDensity Particle density in particles per cubic meter.
         * @return The plasma frequency in radians per second.
         */
        static double calculatePlasmaFrequency(double particleDensity)
        {
            const double numerator = particleDensity * constants::eCharge * constants::eCharge;
            constexpr double denominator = constants::epsilon0 * constants::massDeuterium;
            return std::sqrt(numerator / denominator);
        }

        /**
         * @brief Static method to calculate the Paschen breakdown voltage.
         * @param pressure_Pa Pressure in pascals.
         * @param gap_m Gap distance in meters.
         * @return The Paschen breakdown voltage in volts.
         */
        static double calculatePaschenBreakdown(const double pressure_Pa, const double gap_m)
        {
            const double pd = pressure_Pa * gap_m;
            constexpr double A = 15.0;
            constexpr double B = 365.0;
            constexpr double gamma_se = 0.01;

            const double V_bd = B * pd / (std::log(A * pd) - std::log(std::log(1.0 + 1.0 / gamma_se)));
            return V_bd;
        }

        /**
         * @brief Getter for the ion oscillation period.
         * @return A double representing the ion oscillation period in seconds.
         */
        [[nodiscard]] double getOscillationPeriod() const
        {
            const double T = 2.0 * constants::pi * std::sqrt(
                constants::massDeuterium * m_innerGridRadius * m_outerGridRadius /
                (2.0 * constants::eCharge * std::abs(m_cathodeVoltage)));
            return T;
        }

        /**
         * @brief Calculate the mean free path for deuterium at given pressure.
         * @param pressure_Pa Pressure in pascals.
         * @param temperature_K Temperature in Kelvin.
         * @return Mean free path in meters.
         */
        [[nodiscard]] static double calculateMeanFreePath(const double pressure_Pa, const double temperature_K)
        {
            constexpr double sigma_d = 2.0e-19;
            const double n = pressure_Pa / (constants::kBoltzmann * temperature_K);
            return 1.0 / (std::sqrt(2.0) * sigma_d * n);
        }

        /**
         * @brief Calculate the ionization cross-section for deuterium.
         * @param energy_eV Electron energy in electron volts.
         * @return Ionization cross-section in square meters.
         */
        [[nodiscard]] static double calculateIonizationCrossSection(const double energy_eV)
        {
            constexpr double E_ionization = 13.6;
            if (energy_eV < E_ionization)
            {
                return 0.0;
            }
            const double u = energy_eV / E_ionization;
            constexpr double a0 = 5.29e-11;
            const double sigma = constants::pi * a0 * a0 * (4.0 / u) * std::log(u);
            return sigma;
        }

        /**
         * @brief Calculate the actual grid transparency based on wire geometry.
         * @return Effective grid transparency (0.0 to 1.0).
         */
        [[nodiscard]] double calculateEffectiveTransparency() const
        {
            if (m_gridType == GridType::ROSENSTIEHL_SPHERICAL)
            {
                const double wireArea = m_innerGridWireCount * m_wireDiameter * constants::pi * m_innerGridRadius;
                const double sphereArea = 4.0 * constants::pi * m_innerGridRadius * m_innerGridRadius;
                return 1.0 - (wireArea / sphereArea);
            }
            return m_gridTransparency;
        }

        /**
         * @brief Determine the current operating mode based on pressure and current.
         * @param current_A Current in amperes.
         * @return Operating mode enum value.
         */
        [[nodiscard]] OperatingMode determineOperatingMode(const double current_A) const
        {
            constexpr double PRESSURE_IGNITION_THRESHOLD = 300.0;
            constexpr double PRESSURE_STAR_MODE_THRESHOLD = 3.0;
            constexpr double CURRENT_PLASMA_THRESHOLD = 0.1;

            if (m_operatingPressure > PRESSURE_IGNITION_THRESHOLD)
            {
                return OperatingMode::VACUUM_PUMPING;
            }
            if (m_operatingPressure >= PRESSURE_IGNITION_THRESHOLD && current_A > CURRENT_PLASMA_THRESHOLD)
            {
                return OperatingMode::PLASMA_IGNITION;
            }
            if (m_operatingPressure < PRESSURE_STAR_MODE_THRESHOLD && current_A < CURRENT_PLASMA_THRESHOLD)
            {
                return OperatingMode::STAR_MODE;
            }
            return OperatingMode::STABLE_OPERATION;
        }

        /**
         * @brief Calculate the instantaneous current in the resonant circuit.
         * @param time_s Time in seconds.
         * @param phase_rad Phase offset in radians.
         * @return Instantaneous current in amperes.
         */
        [[nodiscard]] double calculateResonantCurrent(const double time_s, const double phase_rad = 0.0) const
        {
            const double omega = 2.0 * constants::pi * m_resonantFrequency;
            return (m_peakToPeakCurrent / 2.0) * std::sin(omega * time_s + phase_rad);
        }

        /**
         * @brief Getter for wire diameter.
         * @return Wire diameter in meters.
         */
        [[nodiscard]] double getWireDiameter() const { return m_wireDiameter; }

        /**
         * @brief Getter for inner grid wire count.
         * @return Number of wires in the inner grid.
         */
        [[nodiscard]] int getInnerGridWireCount() const { return m_innerGridWireCount; }

        /**
         * @brief Getter for outer grid wire count.
         * @return Number of wires in the outer grid.
         */
        [[nodiscard]] int getOuterGridWireCount() const { return m_outerGridWireCount; }

        /**
         * @brief Getter for grid type.
         * @return Grid type enum value.
         */
        [[nodiscard]] GridType getGridType() const { return m_gridType; }

        /**
         * @brief Getter for resonant frequency.
         * @return Resonant frequency in Hz.
         */
        [[nodiscard]] double getResonantFrequency() const { return m_resonantFrequency; }

        /**
         * @brief Getter for peak-to-peak current.
         * @return Peak-to-peak current in amperes.
         */
        [[nodiscard]] double getPeakToPeakCurrent() const { return m_peakToPeakCurrent; }

        /**
         * @brief Setter for operating pressure.
         * @param pressure_Pa Pressure in pascals.
         */
        void setOperatingPressure(const double pressure_Pa) { m_operatingPressure = pressure_Pa; }

        /**
         * @brief Getter for operating pressure.
         * @return Operating pressure in pascals.
         */
        [[nodiscard]] double getOperatingPressure() const { return m_operatingPressure; }

        /**
         * @brief Setter for grid temperature.
         * @param temp_K Temperature in Kelvin.
         */
        void setGridTemperature(const double temp_K) { m_currentGridTemp = temp_K; }

        /**
         * @brief Getter for current grid temperature.
         * @return Grid temperature in Kelvin.
         */
        [[nodiscard]] double getGridTemperature() const { return m_currentGridTemp; }

        /**
         * @brief Setter for chamber temperature.
         * @param temp_K Temperature in Kelvin.
         */
        void setChamberTemperature(const double temp_K) { m_currentChamberTemp = temp_K; }

        /**
         * @brief Getter for current chamber temperature.
         * @return Chamber temperature in Kelvin.
         */
        [[nodiscard]] double getChamberTemperature() const { return m_currentChamberTemp; }

        /**
         * @brief Check if grid temperature is within safe operating limits.
         * @return True if temperature is safe, false otherwise.
         */
        [[nodiscard]] bool isGridTemperatureSafe() const { return m_currentGridTemp < m_gridTempLimit; }

        /**
         * @brief Check if chamber temperature is within safe operating limits.
         * @return True if temperature is safe, false otherwise.
         */
        [[nodiscard]] bool isChamberTemperatureSafe() const { return m_currentChamberTemp < m_chamberTempLimit; }

    private:

        /**
         * @brief Calculate the geometry factor based on grid radii.
         * @return The geometry factor as a double.
         */
        [[nodiscard]] double calculateGeometryFactor() const
        {
            const double invRi = 1.0 / m_innerGridRadius;
            const double invRo = 1.0 / m_outerGridRadius;
            return 1.0 / (invRi - invRo);
        }

        /**
         * @brief Calculate the radial electric field at a given radius.
         * @param r The radial distance from the center in meters.
         * @return The radial electric field at the given radius in volts per meter.
         */
        [[nodiscard]] double calculateRadialField(const double r) const
        {
            if (r <= m_innerGridRadius)
            {
                return 0.0;
            }

            if (r <= m_outerGridRadius)
            {
                const double invRi = 1.0 / m_innerGridRadius;
                const double invRo = 1.0 / m_outerGridRadius;

                const double E_r = m_cathodeVoltage / ((invRi - invRo) * r * r);
                return E_r;
            }

            return 0.0;
        }

        double m_innerGridRadius;
        double m_outerGridRadius;
        double m_cathodeVoltage;
        double m_gridTransparency;
        double m_wireDiameter;
        int m_innerGridWireCount;
        int m_outerGridWireCount;
        GridType m_gridType;
        double m_resonantFrequency;
        double m_peakToPeakCurrent;
        double m_operatingPressure;
        double m_gridTempLimit;
        double m_chamberTempLimit;
        double m_currentGridTemp;
        double m_currentChamberTemp;
        double m_geometryFactor;
    };
}
