#include "Proton.h"

// Time taken for light to travel 1 um = 3.33564095*10^-15 seconds
// Charge of 1 proton = 1.602*10^-19 (electron *-1) Coulombs
// Mass of a proton =  1.6726*10^-27 kg
// Mass of an electron = 9.10938188*10^-31 kg

const Mass Proton::m_mass = 0.00000000000000000000000000167262158;
const Charge Proton::m_charge = 0.0000000000000000001602;
const Time Proton::m_time_step = 0.00000001;

VPoint Proton::move(const VPoint& p, const VElectricField& v)
{
    // F=mA->A=F/m
    // D=V1*t + 0.5*A*t*t

    const VForce F = VForce::from(v, m_charge);

    const VAcceleration a(
        Acceleration::from(F.x, m_mass),
        Acceleration::from(F.y, m_mass),
        Acceleration::from(F.z, m_mass));

    VPoint d;
    d.x.value =
        (m_velocity.x.value * m_time_step.value)
         + (0.5 * a.x.value * m_time_step.value * m_time_step.value);
    d.y.value =
        (m_velocity.y.value * m_time_step.value)
         + (0.5 * a.y.value * m_time_step.value * m_time_step.value);
    d.z.value =
        (m_velocity.z.value * m_time_step.value)
         + (0.5 * a.z.value * m_time_step.value * m_time_step.value);

    return d;
}
