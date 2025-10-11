#ifndef POINTMAP_H_INCLUDED
#define POINTMAP_H_INCLUDED

#include "Quantities.h"
#include "Vector3d.h"

class PointMap
{
public:
    PointMap(const Potential& p) : m_potential(p) {}

    VElectricField operator()(const VPoint& p) const
    {
        VElectricField E = VElectricField::from(p, m_potential);

        return E;
    }

private:
    const Potential m_potential;
};

#endif // POINTMAP_H_INCLUDED
