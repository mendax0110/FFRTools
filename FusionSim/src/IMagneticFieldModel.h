#pragma once
#include "Vector3dSimple.h"

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Interface for Magnetic Field Models. \class IMagneticFieldModel
    class IMagneticFieldModel
    {
    public:
        /**
         * @brief Virtual destructor for IMagneticFieldModel.
         */
        virtual ~IMagneticFieldModel() = default;

        /**
         * @brief Getter for the magnetic field at a given position.
         * @param position The position where the field is queried.
         * @return The magnetic field vector at the given position.
         */
        [[nodiscard]] virtual Vector3d getFieldAt(const Vector3d& position) const = 0;
    };
}
