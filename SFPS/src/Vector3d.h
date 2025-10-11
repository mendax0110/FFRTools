#ifndef VECTOR3D_H_INCLUDED
#define VECTOR3D_H_INCLUDED

#include <iostream>
#include <cmath>
#include <type_traits>
#include "Quantities.h"

template <typename T, typename = void>
struct quantity_type_of { using type = void; };

template <typename T>
struct quantity_type_of<T, std::void_t<typename T::quantity_t>>
{
    using type = typename T::quantity_t;
};

template <typename T, typename = void>
struct unit_type_of { using type = void; };

template <typename T>
struct unit_type_of<T, std::void_t<typename T::unit_t>>
{
    using type = typename T::unit_t;
};

template <typename Scalar>
struct Vector3d
{
    using scalar_t   = Scalar;
    using quantity_t = typename quantity_type_of<Scalar>::type;
    using unit_t     = typename unit_type_of<Scalar>::type;

    scalar_t x;
    scalar_t y;
    scalar_t z;

    Vector3d() : x(0), y(0), z(0) {}
    Vector3d(const Vector3d&) = default;
    Vector3d(scalar_t px, scalar_t py, scalar_t pz) : x(px), y(py), z(pz) {}

    scalar_t& getX() { return x; }
    scalar_t& getY() { return y; }
    scalar_t& getZ() { return z; }
    const scalar_t& getX() const { return x; }
    const scalar_t& getY() const { return y; }
    const scalar_t& getZ() const { return z; }

    Vector3d normalized() const
    {
        double norm;
        if constexpr (requires { x.value; })
        {
            norm = std::sqrt(x.value * x.value + y.value * y.value + z.value * z.value);
            if (norm == 0) return *this;
            return Vector3d(x / norm, y / norm, z / norm);
        }
        else
        {
            norm = std::sqrt(x * x + y * y + z * z);
            if (norm == 0) return *this;
            return Vector3d(x / norm, y / norm, z / norm);
        }
    }

    auto squaredNorm() const
    {
        return x * x + y * y + z * z;
    }

    double norm() const
    {
        if constexpr (requires { x.value; })
        {
            return std::sqrt(x.value * x.value + y.value * y.value + z.value * z.value);
        }
        else
        {
            return std::sqrt(x * x + y * y + z * z);
        }
    }

    template <typename T>
    Vector3d<scalar_t>& operator=(const Vector3d<T>& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    scalar_t scalar() const
    {
        if constexpr (requires { x.value; })
        {
            scalar_t s;
            s.value = std::sqrt(std::abs(x.value * x.value + y.value * y.value + z.value * z.value));
            return s;
        }
        else
        {
            return std::sqrt(std::abs(x * x + y * y + z * z));
        }
    }

    Vector3d cross(const Vector3d& rhs) const
    {
        return {
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        };
    }

    scalar_t dot(const Vector3d& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    template <typename T1>
    static Vector3d<scalar_t> from(const T1& param1)
    {
        return Vector3d<scalar_t>(scalar_t(param1.x.value), scalar_t(param1.y.value), scalar_t(param1.z.value));
    }

    template <typename T1, typename T2>
    static Vector3d<scalar_t> from(const T1& param1, const T2& param2)
    {
        // Kraft = Ladung * Geschwindigkeit
        if constexpr (std::is_same_v<typename quantity_type_of<scalar_t>::type, quantity_force> &&
                      std::is_same_v<typename quantity_type_of<T1>::type, quantity_velocity> &&
                      std::is_same_v<typename quantity_type_of<T2>::type, quantity_electric_charge>)
        {
            return Vector3d<scalar_t>(
                    scalar_t(param2.value * param1.x.value),
                    scalar_t(param2.value * param1.y.value),
                    scalar_t(param2.value * param1.z.value));
        }
        //Beschleunigung = Kraft / Masse
        if constexpr (std::is_same_v<typename quantity_type_of<scalar_t>::type, quantity_acceleration> &&
                      std::is_same_v<typename quantity_type_of<T1>::type, quantity_force> &&
                      std::is_same_v<typename quantity_type_of<T2>::type, quantity_mass>)
        {
            return Vector3d<scalar_t>(
                    scalar_t(param1.x.value / param2.value),
                    scalar_t(param1.y.value / param2.value),
                    scalar_t(param1.z.value / param2.value));
        }
        // Kopieren der Werte
        return Vector3d<scalar_t>(
                scalar_t(param1.x.value),
                scalar_t(param1.y.value),
                scalar_t(param1.z.value));
    }

    template <typename T1, typename T2, typename T3>
    static Vector3d<scalar_t> from(const T1& param1, const T2& param2, const T3& param3)
    {
        return Vector3d<scalar_t>(scalar_t(param1.x.value), scalar_t(param2.y.value), scalar_t(param3.z.value));
    }

    Vector3d& operator+=(const Vector3d& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    template<typename OtherScalar>
    Vector3d& operator+=(const Vector3d<OtherScalar>& rhs)
    {
        x.value += rhs.x.value;
        y.value += rhs.y.value;
        z.value += rhs.z.value;
        return *this;
    }

    static Vector3d fromDouble(double px, double py, double pz)
    {
        return Vector3d(Scalar(px), Scalar(py), Scalar(pz));
    }

    template<typename OtherScalar>
    static Vector3d fromOther(const Vector3d<OtherScalar>& other)
    {
        return Vector3d(Scalar(other.x.value), Scalar(other.y.value), Scalar(other.z.value));
    }
};

template <typename U>
std::ostream& operator<<(std::ostream& os, const Vector3d<U>& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

template <typename U, typename T>
Vector3d<U> operator+(const Vector3d<U>& lhs, const Vector3d<T>& rhs)
{
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

template <typename U, typename T>
Vector3d<U> operator-(const Vector3d<U>& lhs, const Vector3d<T>& rhs)
{
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

template <typename U, typename T>
Vector3d<U> operator*(const Vector3d<U>& v, const T& scalar)
{
    return { v.x * scalar, v.y * scalar, v.z * scalar };
}

template <typename T, typename U>
Vector3d<U> operator*(const T& scalar, const Vector3d<U>& v)
{
    return { v.x * scalar, v.y * scalar, v.z * scalar };
}

template <typename U, typename T>
Vector3d<U> operator/(const Vector3d<U>& v, const T& scalar)
{
    return { v.x / scalar, v.y / scalar, v.z / scalar };
}

template <typename U>
Vector3d<U> operator-(const Vector3d<U>& v)
{
    return Vector3d<U>(-v.x, -v.y, -v.z);
}

inline Vector3d<double> operator*(double scalar, const Vector3d<double>& v)
{
    return v * scalar;
}

#endif // VECTOR3D_H_INCLUDED

