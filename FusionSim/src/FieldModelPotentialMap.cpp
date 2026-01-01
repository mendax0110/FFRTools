#include "FieldModelPotentialMap.h"
#include "Vector3dSimple.h"

FieldModelPotentialMap::FieldModelPotentialMap(const PointMap& map)
    : m_map(map)
{}

Vector3d FieldModelPotentialMap::getFieldAt(const Vector3d& position) const
{
    // Convert simple Vector3d to SFPS VPoint
    Position px, py, pz;
    px.value = position.x;
    py.value = position.y;
    pz.value = position.z;
    VPoint p(px, py, pz);

    // Get field from map
    VElectricField E = m_map(p);

    // Convert back to simple Vector3d
    return Vector3d(E.x.value, E.y.value, E.z.value);
}