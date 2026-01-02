#if !defined(CALCULATORS_H)
#define CALCULATORS_H

#include <cmath>
#include <algorithm>
#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

/// @brief Potential Calculation Utilities. \namespace Calculators
namespace Calculators
{
    /// @brief Potential Calculation Utilities. \class PotentialCalulator
    class PotentialCalulator
    {
    public:

        /**
         * @brief Calculate the distance from a point to the surface of a sphere.
         * @param r The radius of the sphere.
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @param z The z-coordinate of the point.
         * @return The distance from the point to the surface of the sphere.
         */
        static double calcDist(double r, double x, double y, double z);

        /**
         * @brief Calculate the electric potential at a distance from a charged sphere.
         * @param dist The distance from the center of the sphere.
         * @param voltage The voltage of the sphere.
         * @return The electric potential at the given distance.
         */
        static double calcPotential(double dist, double voltage);

        /**
         * @brief Calculate the electric potential at a point in space due to a charged sphere.
         * @param x The x-coordinate of the point.
         * @param y The y-coordinate of the point.
         * @param z The z-coordinate of the point.
         * @param radius The radius of the sphere.
         * @param kV The voltage of the sphere in kilovolts.
         * @return The electric potential at the given point.
         */
        static double calcPotentialAtPoint(double x, double y, double z, double radius, double kV);

        typedef boost::numeric::ublas::vector< long double > dVector;

        /**
         * @brief Convert to Scalar
         * @param v The dVector to convert.
         * @return A long double representing the scalar value.
         */
        static long double toScalar(const dVector &v);
        static constexpr long double k = 8987551787.368176;
    };
}

#endif // CALCULATORS_H
