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

namespace GeneralEE
{
    class ElectricalEngineering
    {
    public:
        void calculateChamberParameters(const std::string& filename, double pressure, double temperature, double cross_section, double energy);
        void drawLine(gil::rgb8_view_t img, int x1, int y1, int x2, int y2, gil::rgb8_pixel_t color);
    private:

    protected:
    };

    class HighVoltagePowerSupply
    {
    public:
        HighVoltagePowerSupply(double initialPrimaryVoltage, double initialFusorImpedance);

        void simulateOperation();
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

    class FusionRate
    {
    public:
        FusionRate(double pressure, double temperature, double current, double recirculation, double meanVelocity, double gamma, double fusionReactivity);

        double calculateFusionRate();
        double calculateNeutronProductionRate();

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