#ifndef PROTON_H_INCLUDED
#define PROTON_H_INCLUDED

#include "Quantities.h"

/// @brief Class Representing a Proton. \class Proton
class Proton
{
public:
    /**
     * @brief Move the proton based on its current position and the electric field.
     * @param p The current position.
     * @param v The electric field at the current position.
     * @return A new position after movement.
     */
    VPoint move(const VPoint& p, const VElectricField& v);

private:
    VPoint m_velocity;
    static const Charge m_charge;
    static const Mass m_mass;
    static const Time m_time_step;
};

#endif // PROTON_H_INCLUDED
