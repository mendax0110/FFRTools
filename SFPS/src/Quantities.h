#ifndef QUANTITIES_H_INCLUDED
#define QUANTITIES_H_INCLUDED

#include "Scalar.h"

struct quantity_time {};
struct quantity_mass {};
struct quantity_position {};
struct quantity_velocity {};
struct quantity_acceleration {};
struct quantity_force {};

struct quantity_electric_potential {};
struct quantity_electric_charge {};

typedef Scalar< quantity_mass > Mass;
typedef Scalar< quantity_time > Time;
typedef Scalar< quantity_position > Position;
typedef Scalar< quantity_velocity > Velocity;
typedef Scalar< quantity_acceleration > Acceleration;
typedef Scalar< quantity_force > Force;

typedef Scalar< quantity_electric_potential > Potential;
typedef Scalar< quantity_electric_charge > Charge;

template <typename Scalar>
struct Vector3d;

typedef Vector3d< Position > VPoint;
typedef Vector3d< Velocity > VVelocity;
typedef Vector3d< Acceleration > VAcceleration;
typedef Vector3d< Force > VForce;
typedef Vector3d< Potential > VElectricField;


#endif // QUANTITIES_H_INCLUDED
