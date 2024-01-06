#include <cmath>
#include <algorithm>
#include <iostream>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "Calculators.h"

const long double k = 8987551787.368176;

typedef boost::numeric::ublas::vector< long double > dVector;

long double toScalar(const dVector &v)
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

dVector getPosition(double x, double y, double z)
{
	dVector v(3);
	v(0) = x;
	v(1) = y;
	v(2) = z;
	
	return v;
}

dVector getArcPos(const dVector &P, const double radius)
{
	dVector A = getPosition(P(0), P(1), 0.0);
	
	double s = toScalar(A);
	
	if(0.0 != s)
	{
		A /= toScalar(A);
		A *= (radius / 1000); // need to account for the fact that radius is in mm.
	}
	
	return A;
}

double calcDist(const double r, const double x, const double y, const double z)
{
	dVector P = getPosition(x, y, z);
	dVector A = getArcPos(P, r);
	dVector D = A-P;
	
	double dist = toScalar(D);

	return dist;
}

dVector calcField(const dVector &R, const double r)
{
	static const long double C = 1.602176487 * pow(10, -19);	
	dVector E = k * (C / (r*r)) * (R / toScalar(R));
	return E;
}

double calcPotential(const double dist, const double voltage)
{
	double p = k * dist;
	
	return p;
}

double calcPotentialAtPoint(const double x, const double y, const double z, const double radius, const double kV)
{
	// find distance from North-South radial
	const double ns_dist = calcDist(radius, x, y, z);
	
	// find distance from East-West radial
	const double ew_dist = calcDist(radius, y, z, x);
	
	// find distance from lateral
	const double lat_dist = calcDist(radius, z, x, y);
	
	// for each of those, find the smallest distance
	const double min_dist = std::min(ns_dist, std::min(ew_dist, lat_dist));
	double potential = kV / min_dist;
	
	return static_cast< double >(potential);
}
