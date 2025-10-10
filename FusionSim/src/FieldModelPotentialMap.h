#pragma once
#include "IFieldModel.h"
#include "PotentialMap/src/GeneralEE.h"
#include "PotentialMap/src/Calculators.h"

#include "PotentialMap/src/PointMap.h"
#include "PotentialMap/src/GeneralEE.h"
#include "PotentialMap/src/Calculators.h"

class FieldModelPotentialMap : public IFieldModel
{
public:
    FieldModelPotentialMap(const Potential& pot) : m_potential(pot), m_map(pot) {}
    Vector3d getFieldAt(const Vector3d& position) const override
    {
        VPoint p(position.x(), position.y(), position.z());
        VElectricField E = m_map(p);
        return Vector3d(E.x(), E.y(), E.z());
    }
    
private:
    Potential m_potential;
    PointMap m_map;
};
