#pragma once
#include "IMagneticFieldModel.h"

class MagneticFieldUniform : public IMagneticFieldModel
{
public:
    MagneticFieldUniform(const Vector3d<Velocity>& B) : m_B(B) {}
    Vector3d<Velocity> getFieldAt(const Vector3d<Position>&) const override { return m_B; }
private:
    Vector3d<Velocity> m_B;
};
