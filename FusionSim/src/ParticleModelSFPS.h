#pragma once
#include "IParticleModel.h"
#include "../../SFPS/src/ParticleModel.h"
#include "IFieldModel.h"
#include "IMagneticFieldModel.h"
#include <cmath>
#include "../../SFPS/src/Vector3d.h"

class ParticleModelSFPS : public IParticleModel
{
public:
    ParticleModelSFPS(const Vector3d<Position>& pos, const Vector3d<Velocity>& vel, Mass mass, Charge charge, const IFieldModel* field, const IMagneticFieldModel* magfield)
        : position(pos)
        , velocity(vel)
        , m_mass(mass)
        , m_charge(charge)
        , m_field(field)
        , m_magfield(magfield)
        {}

    // runge-kutta 4. Ordnung für bewegugsgleichug mit E- und B-Feld
    void propagate(double dt) override
    {
        // RK4 für Geschwindigkeit und Position
        auto rhs = [&](const Vector3d<Position>& r, const Vector3d<Velocity>& v) -> Vector3d<Velocity>
        {
            Vector3d<Potential> E = m_field ? m_field->getFieldAt(r) : Vector3d<Potential>(Potential(0), Potential(0), Potential(0));
            Vector3d<Velocity> B = m_magfield ? m_magfield->getFieldAt(r) : Vector3d<Velocity>(Velocity(0), Velocity(0), Velocity(0));
            Vector3d<Force> F = VForce::from(E, m_charge) + VForce::from(v.cross(B), m_charge);
            return VVelocity::from(VAcceleration::from(F, m_mass));
        };

        Vector3d<Velocity> k1v = rhs(position, velocity);
        Vector3d<Position> k1r = Vector3d<Position>::fromOther(velocity);

        Vector3d<Velocity> k2v = rhs(position + k1r * (dt/2.0), velocity + k1v * (dt/2.0));
        Vector3d<Position> k2r = Vector3d<Position>::fromOther(velocity + k1v * (dt/2.0));

        Vector3d<Velocity> k3v = rhs(position + k2r * (dt/2.0), velocity + k2v * (dt/2.0));
        Vector3d<Position> k3r = Vector3d<Position>::fromOther(velocity + k2v * (dt/2.0));

        Vector3d<Velocity> k4v = rhs(position + k3r * dt, velocity + k3v * dt);
        Vector3d<Position> k4r = Vector3d<Position>::fromOther(velocity + k3v * dt);

        position = position + (k1r + k2r*2.0 + k3r*2.0 + k4r) * (dt/6.0);
        velocity = velocity + (k1v + k2v*2.0 + k3v*2.0 + k4v) * (dt/6.0);
    }

    Vector3d<Position> getPosition() const override { return position; }
    Vector3d<Velocity> getVelocity() const override { return velocity; }
    void setVelocity(const Vector3d<Velocity>& v) override { velocity = v; }
    Mass getMass() const override { return m_mass; }
    Charge getCharge() const override { return m_charge; }

    IParticleModel* clone() const override
    {
        return new ParticleModelSFPS(*this);
    }
private:
    Vector3d<Position> position;
    Vector3d<Velocity> velocity;
    Mass m_mass;
    Charge m_charge;
    const IFieldModel* m_field;
    const IMagneticFieldModel* m_magfield;
};
