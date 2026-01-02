#pragma once

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Physical constants used in the simulation. \namespace  constants
    namespace constants
    {
        constexpr double kBoltzmann = 1.380649e-23;
        constexpr double eCharge = 1.602176634e-19;
        constexpr double cLight = 299792458.0;
        constexpr double epsilon0 = 8.8541878128e-12;
        constexpr double mu0 = 1.25663706212e-6;
        constexpr double kCoulomb = 8.9875517923e9;
        constexpr double pi = 3.14159265358979323846;

        constexpr double massProton = 1.67262192369e-27;
        constexpr double massNeutron = 1.67492749804e-27;
        constexpr double massElectron = 9.1093837015e-31;
        constexpr double massAMU = 1.66053906660e-27;

        constexpr double massDeuterium = 2.014101778 * massAMU;
        constexpr double massTritium = 3.01604928 * massAMU;
        constexpr double massHe3 = 3.0160293 * massAMU;
        constexpr double massHe4 = 4.00260325413 * massAMU;

        constexpr double eVtoJoule = eCharge;
        constexpr double keVtoJoule = eCharge * 1.0e3;
        constexpr double MeVtoJoule = eCharge * 1.0e6;

        constexpr double barn = 1.0e-28;
        constexpr double millibarn = 1.0e-31;

        /// @brief Constants for the DD reactions. \namespace  dd_reaction
        namespace dd_reaction
        {
            constexpr double Q_He3_n = 3.269;
            constexpr double E_He3 = 0.82;
            constexpr double E_neutron_He3 = 2.45;
            constexpr double Q_T_p = 4.033;
            constexpr double E_Tritium = 1.01;
            constexpr double E_proton = 3.02;

            constexpr double BG = 31.3970;
            constexpr double A1 = 5.3701e4;
            constexpr double A2 = 3.3027e2;
            constexpr double A3 = -1.2706e-1;
            constexpr double A4 = 2.9327e-5;
            constexpr double A5 = -2.5151e-9;
        }

        /// @brief Constants for the DT reaction. \namespace dt_reaction
        namespace dt_reaction
        {
            constexpr double Q_value = 17.586;
            constexpr double E_He4 = 3.52;
            constexpr double E_neutron = 14.07;

            constexpr double BG = 34.3827;
            constexpr double A1 = 6.927e4;
            constexpr double A2 = 7.454e8;
            constexpr double A3 = 2.050e6;
            constexpr double A4 = 5.2002e4;
            constexpr double A5 = 0.0;
        }
    }
}
