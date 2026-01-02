#ifndef POINTMAP_H_INCLUDED
#define POINTMAP_H_INCLUDED

#include "Quantities.h"

/// @brief Point Mapping Utilities. \class PointMap
class PointMap
{
public:

    /**
     * @brief Constructor for PointMap.
     * @param p The electric potential.
     */
    explicit PointMap(const Potential& p) : m_potential(p) {}

    /**
     * @brief Overloaded function call operator to get electric field at a point.
     * @param p The point in space.
     * @return The electric field at the given point.
     */
    VElectricField operator()(const VPoint& p)
    {
        VElectricField E = VElectricField::from(p, m_potential);

        return E;
    }

private:
    const Potential m_potential;
};

#endif // POINTMAP_H_INCLUDED
