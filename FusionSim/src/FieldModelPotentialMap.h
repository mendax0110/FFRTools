#pragma once
#include "IFieldModel.h"
#include "../../SFPS/src/PointMap.h"

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief Field Model based on a potential map. \class FieldModelPotentialMap
    class FieldModelPotentialMap : public IFieldModel
    {
    public:

        /**
         * @brief Constructor for FieldModelPotentialMap.
         * @param map The PointMap containing potential values.
         */
        explicit FieldModelPotentialMap(const PointMap& map);

        /**
         * @brief Constructor for FieldModelPotentialMap with uniform potential.
         * @param potentialValue The uniform potential value.
         */
        explicit FieldModelPotentialMap(double potentialValue);

        /**
         * @brief Getter for the electric field at a given position.
         * @param position The position where the field is queried.
         * @return The electric field vector at the given position.
         */
        [[nodiscard]] fusion::Vector3d getFieldAt(const Vector3d& position) const override;

    private:
        PointMap m_map;
    };
}