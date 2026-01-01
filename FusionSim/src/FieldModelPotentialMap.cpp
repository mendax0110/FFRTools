#include "FieldModelPotentialMap.h"
#include "Vector3dSimple.h"
#include "Quantities.h"

using namespace fusion;

FieldModelPotentialMap::FieldModelPotentialMap(const PointMap& map)
    : m_map(map)
{
}

FieldModelPotentialMap::FieldModelPotentialMap(const double potentialValue)
    : m_map(Potential(potentialValue))
{
}

fusion::Vector3d FieldModelPotentialMap::getFieldAt(const Vector3d& position) const
{
    const Position px(position.x);
    const Position py(position.y);
    const Position pz(position.z);
    const VPoint p(px, py, pz);

    const VElectricField E = const_cast<PointMap&>(m_map)(p);

    return Vector3d(E.x.value, E.y.value, E.z.value);
}