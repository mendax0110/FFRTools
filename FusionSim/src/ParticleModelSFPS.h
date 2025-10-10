#pragma once
#include "IParticleModel.h"
#include "SFPS/src/ParticleModel.h"
#include "IFieldModel.h"
#include "IMagneticFieldModel.h"
#include <cmath>

class ParticleModelSFPS : public IParticleModel
{
public:
    ParticleModelSFPS(const Vector3d& pos, const Vector3d& vel, double mass, double charge, const IFieldModel* field, const IMagneticFieldModel* magfield)
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
        auto rhs = [&](const Vector3d& r, const Vector3d& v)
        {
            Vector3d E = m_field ? m_field->getFieldAt(r) : Vector3d(0,0,0);
            Vector3d B = m_magfield ? m_magfield->getFieldAt(r) : Vector3d(0,0,0);
            Vector3d F = m_charge * (E + v.cross(B));
            return F / m_mass;
        };
        // RK4 für geschw.
        Vector3d k1v = rhs(position, velocity);
        Vector3d k1r = velocity;
        Vector3d k2v = rhs(position + 0.5*dt*k1r, velocity + 0.5*dt*k1v);
        Vector3d k2r = velocity + 0.5*dt*k1v;
        Vector3d k3v = rhs(position + 0.5*dt*k2r, velocity + 0.5*dt*k2v);
        Vector3d k3r = velocity + 0.5*dt*k2v;
        Vector3d k4v = rhs(position + dt*k3r, velocity + dt*k3v);
        Vector3d k4r = velocity + dt*k3v;
        position += (dt/6.0)*(k1r + 2.0*k2r + 2.0*k3r + k4r);
        velocity += (dt/6.0)*(k1v + 2.0*k2v + 2.0*k3v + k4v);
    }
    
    Vector3d getPosition() const override { return position; }
    Vector3d getVelocity() const override { return velocity; }
    void setVelocity(const Vector3d& v) override { velocity = v; }
    double getMass() const { return m_mass; }
    double getCharge() const { return m_charge; }
private:
    Vector3d position;
    Vector3d velocity;
    double m_mass;
    double m_charge;
    const IFieldModel* m_field;
    const IMagneticFieldModel* m_magfield;
};
