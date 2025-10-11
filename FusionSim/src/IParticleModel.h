#pragma once
#include <memory>
#include "Vector3d.h"
#include "../../SFPS/src/Quantities.h"

class IParticleModel
{
public:
    virtual ~IParticleModel() = default;
    virtual void propagate(double dt) = 0;
    virtual Vector3d<Position> getPosition() const = 0;
    virtual Vector3d<Velocity> getVelocity() const = 0;
    virtual void setVelocity(const Vector3d<Velocity>& v) = 0;
    virtual Mass getMass() const = 0;
    virtual Charge getCharge() const = 0;
    virtual IParticleModel* clone() const = 0;
};
