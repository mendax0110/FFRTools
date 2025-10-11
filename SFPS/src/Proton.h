#ifndef PROTON_H_INCLUDED
#define PROTON_H_INCLUDED

#include "Quantities.h"
#include "Vector3d.h"

class Proton
{
public:
    VPoint move(const VPoint& p, const VElectricField& v);

private:
    VPoint m_velocity;
    static const Charge m_charge;
    static const Mass m_mass;
    static const Time m_time_step;
};

#endif // PROTON_H_INCLUDED
