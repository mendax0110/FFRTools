#include "GeneralEE.h"

using namespace GeneralEE;
using namespace std;

void ElectricalEngineering::drawLine(gil::rgb8_view_t img, int x1, int y1, int x2, int y2, gil::rgb8_pixel_t color)
{
    // simple bresenhams line algorithm implementation
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        img(x1, y1) = color;

        if ((x1 == x2) && (y1 == y2))
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void ElectricalEngineering::calculateChamberParameters(const std::string& filename, double pressure, double temperature, double cross_section, double energy)
{
    double density, mean_free_path, vacuum_permittivity, energy_J, energy_J2eV;

    DENSITY(pressure, temperature, density);
    MEAN_FREE_PATH(density, cross_section, mean_free_path);
    VACUUM_PERMITTIVITY(mean_free_path, vacuum_permittivity);
    eV2J(energy, energy_J);
    J2eV(energy_J, energy_J2eV);

    double density_normalized = density / 10.0;
    double mean_free_path_normalized = mean_free_path / 10.0;

    gil::rgb8_image_t img(400, 400);

    fill_pixels(view(img), gil::rgb8_pixel_t(255 - static_cast<unsigned char>(density_normalized * 255),
                                         static_cast<unsigned char>(density_normalized * 255),
                                         0));

    int line_position = static_cast<int>(mean_free_path_normalized * 400);
    line_position = std::min(std::max(line_position, 0), 399);
    drawLine(view(img), 0, line_position, 399, line_position, gil::rgb8_pixel_t(255, 255, 255));

    gil::write_view(filename, const_view(img), gil::png_tag());

    ofstream file;
    file.open("chamber_parameters.txt");
    file << "--------- Chamber Parameters ---------" << endl;
    file << "Image saved to: " << filename << endl;
    file << "Density: " << density << " kg/m^3" << endl;
    file << "Mean Free Path: " << mean_free_path << " m" << endl;
    file << "Vacuum Permittivity: " << vacuum_permittivity << " F/m" << endl;
    file << "Energy in Joules: " << energy_J << " J" << endl;
    file << "Energy in eV: " << energy_J2eV << " eV" << endl;
    file << "---------------------------------------" << endl;
    file.close();

    cout << "--------- Chamber Parameters ---------" << endl;
    cout << "Image saved to: " << filename << endl;
    cout << "Density: " << density << " kg/m^3" << endl;
    cout << "Mean Free Path: " << mean_free_path << " m" << endl;
    cout << "Vacuum Permittivity: " << vacuum_permittivity << " F/m" << endl;
    cout << "Energy in Joules: " << energy_J << " J" << endl;
    cout << "Energy in eV: " << energy_J2eV << " eV" << endl;
    cout << "---------------------------------------" << endl;
}

HighVoltagePowerSupply::HighVoltagePowerSupply(double initialPrimaryVoltage, double initialFusorImpedance)
    : primaryVoltage(initialPrimaryVoltage)
    , secondaryVoltage(0.0)
    , capacitorVoltage(0.0)
    , fusorImpedance(initialFusorImpedance)
    , dcOutputVoltage(0.0)
    , rippleVoltage(0.0)
    , dischargeTimeConstant(0.0)
    , bleederResistance(0.0)
    , dischargeTime(0.0)
{}

void HighVoltagePowerSupply::simulateOperation()
{
    // simulate the operation of a high voltage power supply
    dcOutputVoltage = primaryVoltage * (fusorImpedance / (fusorImpedance + 50.0));
    rippleVoltage = dcOutputVoltage * 0.01;
    dischargeTimeConstant = 0.5 * fusorImpedance * 0.000001;
    bleederResistance = 1000000.0;
    dischargeTime = 5.0 * dischargeTimeConstant;

    displayOutput();
}

void HighVoltagePowerSupply::displayOutput() const
{
    cout << "--------- High Voltage Power Supply Output ---------" << endl;
    cout << "Primary Voltage: " << primaryVoltage << " V" << endl;
    cout << "Secondary Voltage: " << secondaryVoltage << " V" << endl;
    cout << "Capacitor Voltage: " << capacitorVoltage << " V" << endl;
    cout << "DC Output Voltage: " << dcOutputVoltage << " V" << endl;
    cout << "Ripple Voltage: " << rippleVoltage << " V" << endl;
    cout << "Discharge Time Constant: " << dischargeTimeConstant << " s" << endl;
    cout << "Bleeder Resistance: " << bleederResistance << " ohms" << endl;
    cout << "Discharge Time: " << dischargeTime << " s" << endl;
    cout << "----------------------------------------------------" << endl;

    ofstream file;
    file.open("hvps_output.txt");
    file << "--------- High Voltage Power Supply Output ---------" << endl;
    file << "Primary Voltage: " << primaryVoltage << " V" << endl;
    file << "Secondary Voltage: " << secondaryVoltage << " V" << endl;
    file << "Capacitor Voltage: " << capacitorVoltage << " V" << endl;
    file << "DC Output Voltage: " << dcOutputVoltage << " V" << endl;
    file << "Ripple Voltage: " << rippleVoltage << " V" << endl;
    file << "Discharge Time Constant: " << dischargeTimeConstant << " s" << endl;
    file << "Bleeder Resistance: " << bleederResistance << " ohms" << endl;
    file << "Discharge Time: " << dischargeTime << " s" << endl;
    file << "----------------------------------------------------" << endl;
}

FusionRate::FusionRate(double pressure, double temperature, double current, double recirculation, double meanVelocity, double gamma, double fusionReactivity)
    : pressure_(pressure), temperature_(temperature), current_(current), recirculation_(recirculation), meanVelocity_(meanVelocity), gamma_(gamma), fusionReactivity_(fusionReactivity)
{
}

double FusionRate::calculateFusionRate()
{
    double volume = 4.0 * M_PI * pow(recirculation_, 2) * current_ / (1.0 + gamma_);
    double fusionRate = pressure_ / (BoltzmannConstant * temperature_) * current_ / ((1.0 + gamma_) * exp(1.0) * 2.0 * recirculation_ * fusionReactivity_ / meanVelocity_);
    
    return fusionRate;
}

double FusionRate::calculateNeutronProductionRate()
{
    double neutronProductionRate = 2.0 * calculateFusionRate();
    return neutronProductionRate;
}