#pragma once
#include <memory>
#include "Vector3dSimple.h"

class IFieldModel
{
public:
    virtual ~IFieldModel() = default;
    virtual Vector3d getFieldAt(const Vector3d& position) const = 0;
};
