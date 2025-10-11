#pragma once
#include <memory>
#include "Vector3d.h"
#include "../../SFPS/src/Quantities.h"

class IFieldModel
{
public:
    virtual ~IFieldModel() = default;
    virtual Vector3d<Potential> getFieldAt(const Vector3d<Position>& position) const = 0;
};
