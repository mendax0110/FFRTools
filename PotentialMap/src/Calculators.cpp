#include "Calculators.h"

using namespace Calculators;


long double PotentialCalulator::toScalar(const dVector &v)
{
	long double val = v(0)*v(0);
	val += v(1)*v(1);
	val += v(2)*v(2);

	if(0.0 != val)
	{
		val = sqrt(val);
	}

	return val;
}

PotentialCalulator::dVector getPosition(double x, double y, double z)
{
	PotentialCalulator::dVector v(3);
	v(0) = x;
	v(1) = y;
	v(2) = z;
	
	return v;
}

PotentialCalulator::dVector getArcPos(const PotentialCalulator::dVector &P, const double radius)
{
	PotentialCalulator::dVector A = getPosition(P(0), P(1), 0.0);
	
	double s = PotentialCalulator::toScalar(A);
	
	if(0.0 != s)
	{
		A /= PotentialCalulator::toScalar(A);
		A *= (radius / 1000); // need to account for the fact that radius is in mm.
	}
	
	return A;
}

double PotentialCalulator::calcDist(const double r, const double x, const double y, const double z)
{
	dVector P = getPosition(x, y, z);
	dVector A = getArcPos(P, r);
	dVector D = A-P;
	
	double dist = toScalar(D);

	return dist;
}

PotentialCalulator::dVector calcField(const PotentialCalulator::dVector &R, const double r)
{
	static const long double C = 1.602176487 * pow(10, -19);	
	PotentialCalulator::dVector E = PotentialCalulator::k * (C / (r*r)) * (R / PotentialCalulator::toScalar(R));
	return E;
}

double PotentialCalulator::calcPotential(const double dist, const double voltage)
{
	double p = k * dist;
	
	return p;
}

double PotentialCalulator::calcPotentialAtPoint(const double x, const double y, const double z, const double radius, const double kV)
{
	// find distance from North-South radial
	const double ns_dist = calcDist(radius, x, y, z);
	
	// find distance from East-West radial
	const double ew_dist = calcDist(radius, y, z, x);
	
	// find distance from lateral
	const double lat_dist = calcDist(radius, z, x, y);
	
	// for each of those, find the smallest distance
	const double min_dist = std::min(ns_dist, std::min(ew_dist, lat_dist));
	const double potential = kV / min_dist;
	
	return static_cast< double >(potential);
}
