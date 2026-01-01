#pragma once
#include <memory>
#include "Vector3dSimple.h"

namespace fusion
{
    class IFieldModel
    {
    public:
        virtual ~IFieldModel() = default;
        [[nodiscard]] virtual Vector3d getFieldAt(const Vector3d& position) const = 0;
    };
}
