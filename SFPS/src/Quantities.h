#ifndef QUANTITIES_H_INCLUDED
#define QUANTITIES_H_INCLUDED

#include "Vector3d.h"
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

template<> template<> Position Position::from(const Velocity& v, const Acceleration& a, const Time& t);
template<> template<> Velocity Velocity::from(const Position& d, const Time& t);
template<> template<> Acceleration Acceleration::from(const Force& f, const Mass& m);
template<> template<> Force Force::from(const Mass& m, const Acceleration& a);

typedef Vector3d< Position > VPoint;
typedef Vector3d< Velocity > VVelocity;
typedef Vector3d< Acceleration > VAcceleration;
typedef Vector3d< Force > VForce;
typedef Vector3d< Potential > VElectricField;

template<>
template<>
VElectricField VElectricField::from(const VPoint& dS, const Potential& dV);
template<> template<> VForce VForce::from(const VElectricField& E, const Charge& q);

#endif // QUANTITIES_H_INCLUDED
