#pragma once
#include "Vector3dSimple.h"

namespace fusion
{
    class IMagneticFieldModel
    {
    public:
        virtual ~IMagneticFieldModel() = default;
        [[nodiscard]] virtual Vector3d getFieldAt(const Vector3d& position) const = 0;
    };
}
