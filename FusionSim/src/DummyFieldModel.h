#pragma once
#include "IFieldModel.h"
#include <cmath>

class DummyFieldModel : public IFieldModel
{
public:
    DummyFieldModel(Potential E0 = Potential(1e5), Position r0 = Position(0.01)) : m_E0(E0), m_r0(r0) {}
    Vector3d<Potential> getFieldAt(const Vector3d<Position>& pos) const override
    {
        double r = std::sqrt(pos.x.value * pos.x.value + pos.y.value * pos.y.value + pos.z.value * pos.z.value);
        if (r == 0.0) return Vector3d<Potential>(Potential(0), Potential(0), Potential(0));
        double Er = m_E0.value * (m_r0.value * m_r0.value) / (r * r);
        return Vector3d<Potential>(Potential(Er * pos.x.value / r), Potential(Er * pos.y.value / r), Potential(Er * pos.z.value / r));
    }
private:
    Potential m_E0; // Feldstärke am Gitter
    Position m_r0; // Gitterradius
};
