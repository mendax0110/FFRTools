#pragma once
#include "Vector3d.h"

class IMagneticFieldModel
{
public:
    virtual ~IMagneticFieldModel() = default;
    virtual Vector3d<Velocity> getFieldAt(const Vector3d<Position>& position) const = 0;
};
