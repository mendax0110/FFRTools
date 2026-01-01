#pragma once
#include "IFieldModel.h"

// Forward declare to avoid including SFPS headers
class PointMap;

class FieldModelPotentialMap : public IFieldModel
{
public:
    FieldModelPotentialMap(const PointMap& map);
    Vector3d getFieldAt(const Vector3d& position) const override;

private:
    const PointMap& m_map;
};