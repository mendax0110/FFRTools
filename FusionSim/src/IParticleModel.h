#pragma once
#include <memory>
#include "Vector3dSimple.h"

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Interface for Particle Models. \class IParticleModel
    class IParticleModel
    {
    public:

        /**
         * @brief Virtual destructor for IParticleModel.
         */
        virtual ~IParticleModel() = default;

        /**
         * @brief Propagate the particle for a time step dt.
         * @param dt Time step for propagation.
         */
        virtual void propagate(double dt) = 0;

        /**
         * @brief Getter for the current position.
         * @return Current position as Vector3d.
         */
        [[nodiscard]] virtual Vector3d getPosition() const = 0;

        /**
         * @brief Getter for the current velocity.
         * @return Current velocity as Vector3d.
         */
        [[nodiscard]] virtual Vector3d getVelocity() const = 0;

        /**
         * @brief Setter for the velocity.
         * @param v New velocity as Vector3d.
         */
        virtual void setVelocity(const Vector3d& v) = 0;

        /**
         * @brief Getter for the particle mass.
         * @return Particle mass as double.
         */
        [[nodiscard]] virtual double getMass() const = 0;

        /**
         * @brief Getter for the particle charge.
         * @return Particle charge as double.
         */
        [[nodiscard]] virtual double getCharge() const = 0;

        /**
         * @brief Clone the particle model.
         * @return A unique pointer to the cloned particle model.
         */
        [[nodiscard]] virtual std::unique_ptr<IParticleModel> clone() const = 0;
    };
}
