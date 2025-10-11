#pragma once
#include "IFieldModel.h"
#include "../../PotentialMap/src/GeneralEE.h"
#include "../../PotentialMap/src/Calculators.h"
#include "../../SFPS/src/PointMap.h"
#include "../../SFPS/src/Quantities.h"
#include "../../SFPS/src/Vector3d.h"

class FieldModelPotentialMap : public IFieldModel
{
public:
    FieldModelPotentialMap(const Potential& pot) : m_potential(pot), m_map(pot) {}
    Vector3d<Potential> getFieldAt(const Vector3d<Position>& position) const override
    {
        VPoint p(position.x.value, position.y.value, position.z.value);
        VElectricField E = m_map(p);
        Vector3d<Potential> field(E.getX(), E.getY(), E.getZ());
        if (std::isnan(field.x.value) ||
            std::isnan(field.y.value) ||
            std::isnan(field.z.value))
        {
            //std::cerr << "[DEBUG] getFieldAt: NaN-Feld an Position (" << position.x.value << ", " << position.y.value << ", " << position.z.value << ")\n";
        }
        return field;
    }
    
private:
    Potential m_potential;
    PointMap m_map;
};
