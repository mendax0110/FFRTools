#ifndef CALCULATIONS_H_INCLUDED
#define CALCULATIONS_H_INCLUDED

#include "Quantities.h"

#define SCALAR_EQUATION(result_type, lhs_type, operand, rhs_type) \
result_type operator##operand(const lhs_type& lhs, const rhs_type& rhs) \
{ \
    result_type result; \
    result.value = lhs.value, rhs.value; \
    return result; \
}

SCALAR_EQUATION(Velocity, Position, /, Time)
SCALAR_EQUATION(Acceleration, Velocity, /, Time)

SCALAR_EQUATION(Force, Mass, *, Acceleration)

#endif // CALCULATIONS_H_INCLUDED
