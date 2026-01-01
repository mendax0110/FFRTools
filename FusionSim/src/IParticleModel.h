#pragma once
#include <memory>
#include "Vector3dSimple.h"

class IParticleModel
{
public:
    virtual ~IParticleModel() = default;
    virtual void propagate(double dt) = 0;
    [[nodiscard]] virtual Vector3d getPosition() const = 0;
    [[nodiscard]] virtual Vector3d getVelocity() const = 0;
    virtual void setVelocity(const Vector3d& v) = 0;

    virtual double getMass() const = 0;
    virtual double getCharge() const = 0;
    virtual std::unique_ptr<IParticleModel> clone() const = 0;
};
