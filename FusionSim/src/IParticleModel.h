#pragma once
#include <memory>
#include "Vector3d.h"

class IParticleModel
{
public:
    virtual ~IParticleModel() = default;
    virtual void propagate(double dt) = 0;
    virtual Vector3d getPosition() const = 0;
    virtual Vector3d getVelocity() const = 0;
    virtual void setVelocity(const Vector3d& v) = 0;
};
