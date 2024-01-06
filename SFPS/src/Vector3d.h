#ifndef VECTOR3D_H_INCLUDED
#define VECTOR3D_H_INCLUDED

#include <iostream>
#include <cmath>

template< typename Scalar >
struct Vector3d
{
    typedef Scalar scalar_t;
    typedef typename scalar_t::quantity_t quantity_t;
    typedef typename scalar_t::unit_t unit_t;

    scalar_t x;
    scalar_t y;
    scalar_t z;

    Vector3d()

        :    x(0)
        ,    y(0)
        ,    z(0)
    {
    }

    Vector3d(const Vector3d& rhs)

        :    x(rhs.x)
        ,    y(rhs.y)
        ,    z(rhs.z)
    {
    }

    Vector3d(scalar_t px, scalar_t py, scalar_t pz)

        :    x(px)
        ,    y(py)
        ,    z(pz)
    {
    }

    template< typename T >
    Vector3d< scalar_t >& operator=(const Vector3d< T >& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;

        return *this;
    }

    scalar_t scalar()
    {
        scalar_t s;
        s.value =
            std::sqrt(std::abs(
                (x.value * x.value)
                 + (y.value * y.value)
                 + (z.value * z.value)));

        return s;
    }

    template< typename T1 >
    static Vector3d< scalar_t > from(const T1& param1);

    template< typename T1, typename T2 >
    static Vector3d< scalar_t > from(const T1& param1, const T2& param2);

    template< typename T1, typename T2, typename T3 >
    static Vector3d< scalar_t > from(const T1& param1, const T2& param2, const T3& param3);
};

template< typename U >
std::ostream& operator<<(std::ostream& os, Vector3d< U > v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

template< typename U, typename T >
Vector3d< U > operator+(const Vector3d< U >& lhs, const Vector3d< T >& rhs)
{
    Vector3d< U > result(
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z);

    return result;
}

template< typename U, typename T >
Vector3d< U > operator+(const Vector3d< U >& v, const T& scalar)
{
    Vector3d< U > result(v);
    result.x.value += scalar.value;
    result.y.value += scalar.value;
    result.z.value += scalar.value;

    return result;
}

template< typename U, typename T >
Vector3d< U > operator*(const Vector3d< U >& v, const T& scalar)
{
    Vector3d< U > result(v);
    result.x.value *= scalar.value;
    result.y.value *= scalar.value;
    result.z.value *= scalar.value;

    return result;
}

template< typename U, typename T >
Vector3d< U > operator-(const Vector3d< U >& v, const T& scalar)
{
    Vector3d< U > result(v);
    result.x.value -= scalar.value;
    result.y.value -= scalar.value;
    result.z.value -= scalar.value;

    return result;
}

template< typename U, typename T >
Vector3d< U > operator/(const Vector3d< U >& v, const T& scalar)
{
    Vector3d< U > result(v);
    result.x.value /= scalar.value;
    result.y.value /= scalar.value;
    result.z.value /= scalar.value;

    return result;
}

#endif // VECTOR3D_H_INCLUDED
