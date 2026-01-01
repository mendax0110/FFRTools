#pragma once
#include "IFieldModel.h"
#include "PointMap.h"

namespace fusion
{
    class FieldModelPotentialMap : public IFieldModel
    {
    public:
        explicit FieldModelPotentialMap(const PointMap& map);
        explicit FieldModelPotentialMap(double potentialValue);

        [[nodiscard]] fusion::Vector3d getFieldAt(const Vector3d& position) const override;

    private:
        PointMap m_map;
    };
}