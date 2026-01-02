#pragma once
#include "IParticleModel.h"
#include "Vector3dSimple.h"
#include "IFieldModel.h"
#include "IMagneticFieldModel.h"
#include <cmath>
#include <memory>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Simple Particle Model using 4th order Runge-Kutta for propagation. \class ParticleModelSFPS
    class ParticleModelSFPS : public IParticleModel
    {
    public:

        /**
         * @brief Constructor for ParticleModelSFPS.
         * @param pos Initial position.
         * @param vel Initial velocity.
         * @param mass Particle mass.
         * @param charge Particle charge.
         * @param field Shared pointer to the electric field model.
         * @param magfield Shared pointer to the magnetic field model.
         */
        ParticleModelSFPS(
            const Vector3d& pos,
            const Vector3d& vel,
            const double mass,
            const double charge,
            std::shared_ptr<const IFieldModel> field,
            std::shared_ptr<const IMagneticFieldModel> magfield)
            : position(pos)
            , velocity(vel)
            , m_mass(mass)
            , m_charge(charge)
            , m_field(std::move(field))
            , m_magfield(std::move(magfield))
        {
        }

        /**
         * @brief Propagate the particle using 4th order Runge-Kutta method.
         * @param dt Time step for propagation.
         */
        void propagate(const double dt) override
        {
            auto rhs = [&](const Vector3d& r, const Vector3d& v) -> Vector3d
            {
                const Vector3d E = m_field ? m_field->getFieldAt(r) : Vector3d(0, 0, 0);
                const Vector3d B = m_magfield ? m_magfield->getFieldAt(r) : Vector3d(0, 0, 0);
                const Vector3d F = m_charge * (E + v.cross(B));
                return F / m_mass;
            };

            const Vector3d k1v = rhs(position, velocity);
            const Vector3d k1r = velocity;
            const Vector3d k2v = rhs(position + 0.5 * dt * k1r, velocity + 0.5 * dt * k1v);
            const Vector3d k2r = velocity + 0.5 * dt * k1v;
            const Vector3d k3v = rhs(position + 0.5 * dt * k2r, velocity + 0.5 * dt * k2v);
            const Vector3d k3r = velocity + 0.5 * dt * k2v;
            const Vector3d k4v = rhs(position + dt * k3r, velocity + dt * k3v);
            const Vector3d k4r = velocity + dt * k3v;

            position += (dt / 6.0) * (k1r + 2.0 * k2r + 2.0 * k3r + k4r);
            velocity += (dt / 6.0) * (k1v + 2.0 * k2v + 2.0 * k3v + k4v);
        }

        /**
         * @brief Getter for the current position.
         * @return Current position as Vector3d.
         */
        [[nodiscard]] Vector3d getPosition() const override
        {
            return position;
        }

        /**
         * @brief Getter for the current velocity.
         * @return Current velocity as Vector3d.
         */
        [[nodiscard]] Vector3d getVelocity() const override
        {
            return velocity;
        }

        /**
         * @brief Setter for the velocity.
         * @param v New velocity as Vector3d.
         */
        void setVelocity(const Vector3d& v) override
        {
            velocity = v;
        }

        /**
         * @brief Getter for the particle mass.
         * @return Particle mass as double.
         */
        [[nodiscard]] double getMass() const override
        {
            return m_mass;
        }

        /**
         * @brief Getter for the particle charge.
         * @return Particle charge as double.
         */
        [[nodiscard]] double getCharge() const override
        {
            return m_charge;
        }

        /**
         * @brief Getter for the electric field model.
         * @return Shared pointer to the electric field model.
         */
        [[nodiscard]] std::shared_ptr<const IFieldModel> getFieldModel() const
        {
            return m_field;
        }

        /**
         * @brief Getter for the magnetic field model.
         * @return Shared pointer to the magnetic field model.
         */
        [[nodiscard]] std::shared_ptr<const IMagneticFieldModel> getMagFieldModel() const
        {
            return m_magfield;
        }

        /**
         * @brief Clone the particle model.
         * @return Unique pointer to a new ParticleModelSFPS instance.
         */
        [[nodiscard]] std::unique_ptr<IParticleModel> clone() const override
        {
            return std::make_unique<ParticleModelSFPS>(position, velocity, m_mass, m_charge, m_field, m_magfield);
        }

    private:
        Vector3d position;
        Vector3d velocity;
        double m_mass;
        double m_charge;
        std::shared_ptr<const IFieldModel> m_field;
        std::shared_ptr<const IMagneticFieldModel> m_magfield;
    };
}