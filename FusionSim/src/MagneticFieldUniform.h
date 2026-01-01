#pragma once
#include "IMagneticFieldModel.h"

namespace fusion
{
    class MagneticFieldUniform : public IMagneticFieldModel
    {
    public:
        explicit MagneticFieldUniform(const Vector3d& B)
            : m_B(B)
        {
        }

        [[nodiscard]] Vector3d getFieldAt(const Vector3d&) const override
        {
            return m_B;
        }

    private:
        Vector3d m_B;
    };
}
