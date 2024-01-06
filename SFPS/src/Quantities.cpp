#include "Quantities.h"


template<> template<> Position Position::from(const Velocity& v, const Acceleration& a, const Time& t)
{
    Position d;
    d.value = (v.value * t.value) + (0.5 * a.value * t.value * t.value);

    return d;
}

template<> template<> Velocity Velocity::from(const Position& d, const Time& t)
{
     Velocity v;
     v.value = (d.value / t.value);
     return v;
}

template<> template<> Acceleration Acceleration::from(const Force& f, const Mass& m)
{
    Acceleration a;
    a.value = f.value / m.value;

    return a;
}

template<> template<> Force Force::from(const Mass& m, const Acceleration& a)
{
    Force f;
    f.value = m.value * a.value;

    return f;
}

template<> template<> VElectricField VElectricField::from(const VPoint& dS, const Potential& dV)
{
    VElectricField E;
    E.x.value = -(dV.value / dS.x.value);
    E.y.value = -(dV.value / dS.y.value);
    E.z.value = -(dV.value / dS.z.value);

    return E;
}

// This should be the lorentz force law proper - but don't really know enough for the
// full implementation.
template<> template<> VForce VForce::from(const VElectricField& E, const Charge& q)
{
    VForce F;
    F.x.value = q.value * E.x.value;
    F.y.value = q.value * E.y.value;
    F.z.value = q.value * E.z.value;

    return F;
}
