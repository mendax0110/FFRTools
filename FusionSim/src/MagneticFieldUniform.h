#pragma once
#include "IMagneticFieldModel.h"

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Uniform Magnetic Field Model. \class MagneticFieldUniform
    class MagneticFieldUniform : public IMagneticFieldModel
    {
    public:

        /**
         * @brief Constructor for MagneticFieldUniform.
         * @param B The uniform magnetic field vector.
         */
        explicit MagneticFieldUniform(const Vector3d& B)
            : m_B(B)
        {
        }

        /**
         * @brief Getter for the magnetic field at a given position.
         * @param position The position where the field is queried.
         * @return The uniform magnetic field vector.
         */
        [[nodiscard]] Vector3d getFieldAt(const Vector3d&) const override
        {
            return m_B;
        }

    private:
        Vector3d m_B;
    };
}
