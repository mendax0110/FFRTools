#ifndef CALCMACROS_H_
#define CALCMACROS_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <math.h>
#include <limits>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

/*
 * @brief The macro for pi
 */
#define PI_Macro 3.1415926535897932384626433832795028841971693993751058209749445923078164062

/*
 * @brief The macro for the electric constant (epsilon_0) in SI units
 */
#define k_Macro 8.9875517873681764 * pow(10, 9)

/*
 * @brief The macro for the electric constant (epsilon_0) in SI units
 */
#define kV_Macro 8.9875517873681764 * pow(10, 3)

/*
 * @brief The macro for coulombs constant (C) in SI units
 */
#define C_Macro 1.602176487 * pow(10, -19)

/*
 * @brief The macro for the mass of an electron (m_e) in SI units
 */
#define m_e_Macro 9.10938215 * pow(10, -31)

/*
 * @brief The macro for the mass of a proton (m_p) in SI units
 */
#define m_p_Macro 1.672621637 * pow(10, -27)

/*
 * @brief The macro for the mass of a neutron (m_n) in SI units
 */
#define m_n_Macro 1.674927211 * pow(10, -27)

/*
 * @brief The macro for deuterium gas (D2) in SI units
 */
#define D2_Macro 3.343583719 * pow(10, -27)

/*
 * @brief The macro to convert from degrees to radians
 */
#define DEG2RAD(degree, radian) \
    radian = degree * (PI / 180);

/*
 * @brief The macro to convert from radians to degrees
 */
#define RAD2DEG(radian, degree) \
    degree = radian * (180 / PI);

/*
 * @brief The macro to convert from electron volts to joules
 */
#define eV2J(eV, J) \
    J = eV * C_Macro;

/*
 * @brief The macro to convert from joules to electron volts
 */
#define J2eV(J, eV) \
    eV = J / C_Macro;

/*
 * @brief The macro to calculate the density of the deuterium gas in the chamber
 */
#define DENSITY(pressure, temperature, density) \
    density = (pressure * D2_Macro) / (temperature * 8.314);

/*
 * @brief The macro to calculate the mean free path of the deuterium gas in the chamber
 */
#define MEAN_FREE_PATH(density, cross_section, mean_free_path) \
    mean_free_path = 1 / ((density * cross_section) * pow(10, -4));

/*
 * @brief The macro to calculate the vacuum permittivity of the deuterium gas in the chamber
 */
#define VACUUM_PERMITTIVITY(mean_free_path, vacuum_permittivity) \
    vacuum_permittivity = (mean_free_path * 1.602176487 * pow(10, -19)) / (8.854187817 * pow(10, -12));


#endif /* CALCMACROS_H_ */