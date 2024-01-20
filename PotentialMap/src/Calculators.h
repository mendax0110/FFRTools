#if !defined(CALCULATORS_H)
#define CALCULATORS_H

#include <cmath>
#include <algorithm>
#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace Calculators
{
    class PotentialCalulator
    {
    public:
        static double calcDist(const double r, const double x, const double y, const double z);
        static double calcPotential(const double dist, const double voltage);
        static double calcPotentialAtPoint(const double x, const double y, const double z, const double radius, const double kV);
        typedef boost::numeric::ublas::vector< long double > dVector;
        static long double toScalar(const dVector &v);
        static constexpr long double k = 8987551787.368176;
    };
}

#endif // CALCULATORS_H
