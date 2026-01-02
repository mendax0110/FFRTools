#pragma once
#include <memory>
#include "Vector3dSimple.h"

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Interface for Field Models. \class IFieldModel
    class IFieldModel
    {
    public:
        /**
         * @brief Virtual destructor for IFieldModel.
         */
        virtual ~IFieldModel() = default;

        /**
         * @brief Getter for the electric field at a given position.
         * @param position The position where the field is queried.
         * @return The electric field vector at the given position.
         */
        [[nodiscard]] virtual Vector3d getFieldAt(const Vector3d& position) const = 0;
    };
}
