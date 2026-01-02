#pragma once
#include "IFieldModel.h"
#include "PhysicalConstants.h"
#include <cmath>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Farnsworth Fusor Field Model. \class FarnsworthFusorFieldModel
    class FarnsworthFusorFieldModel : public IFieldModel
    {
    public:
        /// @brief Default inner grid radius in meters.
        static constexpr double defaultInnerGridRadius = 0.016;

        /// @brief Default outer grid radius in meters.
        static constexpr double defaultOuterGridRadius = 0.08;

        /// @brief Default cathode voltage in volts.
        static constexpr double defaultCathodeVoltage = -30000.0;

        /// @brief Default grid transparency (0.0 to 1.0).
        static constexpr double defaultGridTransparency = 0.95;

        /**
         * @brief Constructor for FarnsworthFusorFieldModel with default parameters.
         */
        FarnsworthFusorFieldModel()
            : m_innerGridRadius(defaultInnerGridRadius)
            , m_outerGridRadius(defaultOuterGridRadius)
            , m_cathodeVoltage(defaultCathodeVoltage)
            , m_gridTransparency(defaultGridTransparency)
            , m_geometryFactor(calculateGeometryFactor())
        {
        }

        /**
         * @brief Constructor for FarnsworthFusorFieldModel with custom parameters.
         * @param innerGridRadius Inner grid (cathode) radius in meters.
         * @param outerGridRadius Outer grid radius in meters.
         * @param cathodeVoltage Cathode voltage in volts.
         * @param gridTransparency Grid transparency (0.0 to 1.0).
         */
        FarnsworthFusorFieldModel(
            const double innerGridRadius,
            const double outerGridRadius,
            const double cathodeVoltage,
            const double gridTransparency = 0.95)
            : m_innerGridRadius(innerGridRadius)
            , m_outerGridRadius(outerGridRadius)
            , m_cathodeVoltage(cathodeVoltage)
            , m_gridTransparency(gridTransparency)
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
        double m_geometryFactor;
    };
}
