#pragma once
#include "Vector3dSimple.h"

class IMagneticFieldModel
{
public:
    virtual ~IMagneticFieldModel() = default;
    virtual Vector3d getFieldAt(const Vector3d& position) const = 0;
};
