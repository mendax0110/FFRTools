#pragma once
#include "IMagneticFieldModel.h"

class MagneticFieldUniform : public IMagneticFieldModel
{
public:
    MagneticFieldUniform(const Vector3d& B) : m_B(B) {}
    Vector3d getFieldAt(const Vector3d&) const override { return m_B; }
private:
    Vector3d m_B;
};
