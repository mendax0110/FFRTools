#if !defined(CALCULATORS_H)
#define CALCULATORS_H

double calcDist(const double r, const double x, const double y, const double z);
double calcPotential(const double dist, const double voltage);

double calcPotentialAtPoint(const double x, const double y, const double z, const double radius, const double kV);

#endif // CALCULATORS_H
