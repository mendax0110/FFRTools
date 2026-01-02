#ifndef GENERAL_EE_H_
#define GENERAL_EE_H_

#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>
#include <boost/gil/extension/dynamic_image/dynamic_image_all.hpp>

#include "CalcMacros.h"

namespace gil = boost::gil;

/// @brief General Electrical Engineering Utilities. \namespace  GeneralEE
namespace GeneralEE
{
    /// @brief Electrical Engineering Utilities. \class ElectricalEngineering
    class ElectricalEngineering
    {
    public:

        /**
         * @brief Calculate chamber parameters and generate visualization.
         * @param filename The output filename for the visualization.
         * @param pressure The pressure in the chamber.
         * @param temperature The temperature in the chamber.
         * @param cross_section The cross section for interactions.
         * @param energy The energy of particles.
         */
        static void calculateChamberParameters(const std::string& filename, double pressure, double temperature, double cross_section, double energy);

        /**
         * @brief Draw a line on an image using Bresenham's algorithm.
         * @param img The image view to draw on.
         * @param x1 Starting x-coordinate.
         * @param y1 Starting y-coordinate.
         * @param x2 Ending x-coordinate.
         * @param y2 Ending y-coordinate.
         * @param color The color of the line.
         */
        static void drawLine(const gil::rgb8_view_t &img, int x1, int y1, int x2, int y2, const gil::rgb8_pixel_t &color);
    };

    /// @brief High Voltage Power Supply Simulation. \class HighVoltagePowerSupply
    class HighVoltagePowerSupply
    {
    public:

        /**
         * @brief Constructor for HighVoltagePowerSupply.
         * @param initialPrimaryVoltage The initial primary voltage.
         * @param initialFusorImpedance The fusor impedance.
         */
        HighVoltagePowerSupply(double initialPrimaryVoltage, double initialFusorImpedance);

        /**
         * @brief Simulate the operation of the high voltage power supply.
         */
        void simulateOperation();

        /**
         * @brief Display the output parameters of the power supply.
         */
        void displayOutput() const;

    private:
        double primaryVoltage;
        double secondaryVoltage;
        double capacitorVoltage;
        double fusorImpedance;
        double dcOutputVoltage;
        double rippleVoltage;
        double dischargeTimeConstant;
        double bleederResistance;
        double dischargeTime;
    };

    /// @brief Fusion Rate Calculation. \class FusionRate
    class FusionRate
    {
    public:

        /**
         * @brief Constructor for FusionRate.
         * @param pressure The pressure in the chamber.
         * @param temperature The temperature in the chamber.
         * @param current The current in the system.
         * @param recirculation The recirculation factor.
         * @param meanVelocity The mean velocity of particles.
         * @param gamma The geometry factor.
         * @param fusionReactivity The fusion reactivity.
         */
        FusionRate(double pressure, double temperature, double current, double recirculation, double meanVelocity, double gamma, double fusionReactivity);

        /**
         * @brief Calculate the fusion rate.
         * @return The fusion rate as a double.
         */
        double calculateFusionRate() const;

        /**
         * @brief Calculate the neutron production rate.
         * @return The neutron production rate as a double.
         */
        double calculateNeutronProductionRate() const;

    private:
        static constexpr double BoltzmannConstant = 1.380649e-23;
        double pressure_;
        double temperature_;
        double current_;
        double recirculation_;
        double meanVelocity_;
        double gamma_;
        double fusionReactivity_;
    };
}

#endif // GENERAL_EE_H_