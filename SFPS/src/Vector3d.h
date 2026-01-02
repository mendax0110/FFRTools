#ifndef VECTOR3D_H_INCLUDED
#define VECTOR3D_H_INCLUDED

#include <iostream>
#include <cmath>

/**
 * @brief 3D Vector Structure. \struct Vector3d
 * @tparam Scalar The scalar type used for the vector components.
 */
template< typename Scalar >
struct Vector3d
{
    typedef Scalar scalar_t;
    typedef typename scalar_t::quantity_t quantity_t;
    typedef typename scalar_t::unit_t unit_t;

    scalar_t x;
    scalar_t y;
    scalar_t z;

    /**
     * @brief Default constructor for Vector3d.
     */
    Vector3d()

        :    x(0)
        ,    y(0)
        ,    z(0)
    {
    }

    /**
     * @brief Copy constructor for Vector3d.
     * @param rhs The vector to copy from.
     */
    Vector3d(const Vector3d& rhs)

        :    x(rhs.x)
        ,    y(rhs.y)
        ,    z(rhs.z)
    {
    }

    /**
     * @brief Parameterized constructor for Vector3d.
     * @param px The x-component.
     * @param py The y-component.
     * @param pz The z-component.
     */
    Vector3d(scalar_t px, scalar_t py, scalar_t pz)

        :    x(px)
        ,    y(py)
        ,    z(pz)
    {
    }

    /**
     * @brief Assignment operator for Vector3d.
     * @tparam T The type of the right-hand side vector.
     * @param rhs The vector to assign from.
     * @return Reference to the assigned vector.
     */
    template< typename T >
    Vector3d< scalar_t >& operator=(const Vector3d< T >& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;

        return *this;
    }

    /**
     * @brief Calculate the scalar magnitude of the vector.
     * @return The scalar magnitude as scalar_t.
     */
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

    /**
     * @brief Calculate the Euclidean norm of the vector.
     * @return The Euclidean norm as double.
     */
    template< typename T1 >
    static Vector3d< scalar_t > from(const T1& param1);

    /**
     * @brief Create a Vector3d from two parameters.
     * @tparam T1 The type of the first parameter.
     * @tparam T2 The type of the second parameter.
     * @param param1 The first parameter.
     * @param param2 The second parameter.
     * @return A Vector3d constructed from the two parameters.
     */
    template< typename T1, typename T2 >
    static Vector3d< scalar_t > from(const T1& param1, const T2& param2);

    /**
     * @brief Create a Vector3d from three parameters.
     * @tparam T1 The type of the first parameter.
     * @tparam T2 The type of the second parameter.
     * @tparam T3 The type of the third parameter.
     * @param param1 The first parameter.
     * @param param2 The second parameter.
     * @param param3 The third parameter.
     * @return A Vector3d constructed from the three parameters.
     */
    template< typename T1, typename T2, typename T3 >
    static Vector3d< scalar_t > from(const T1& param1, const T2& param2, const T3& param3);
};

/**
 * @brief Overloaded output stream operator for Vector3d.
 * @tparam U The scalar type of the vector.
 * @param os The output stream.
 * @param v The vector to output.
 * @return Reference to the output stream.
 */
template< typename U >
std::ostream& operator<<(std::ostream& os, Vector3d< U > v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

/**
 * @brief Overloaded addition operator for Vector3d.
 * @tparam U The scalar type of the left-hand side vector.
 * @tparam T The scalar type of the right-hand side vector.
 * @param lhs The left-hand side vector.
 * @param rhs The right-hand side vector.
 * @return A new Vector3d representing the sum.
 */
template< typename U, typename T >
Vector3d< U > operator+(const Vector3d< U >& lhs, const Vector3d< T >& rhs)
{
    Vector3d< U > result(
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z);

    return result;
}

/**
 * @brief Overloaded subtraction operator for Vector3d.
 * @tparam U The scalar type of the left-hand side vector.
 * @tparam T The scalar type of the right-hand side vector.
 * @param v The left-hand side vector.
 * @param scalar The scalar.
 * @return A new Vector3d representing the difference.
 */
template< typename U, typename T >
Vector3d< U > operator+(const Vector3d< U >& v, const T& scalar)
{
    Vector3d< U > result(v);
    result.x.value += scalar.value;
    result.y.value += scalar.value;
    result.z.value += scalar.value;

    return result;
}

/**
 * @brief Overloaded multiplication operator for Vector3d.
 * @tparam U The scalar type of the left-hand side vector.
 * @tparam T The scalar type of the right-hand side vector.
 * @param v The left-hand side vector.
 * @param scalar The scalar.
 * @return A new Vector3d representing the product.
 */
template< typename U, typename T >
Vector3d< U > operator*(const Vector3d< U >& v, const T& scalar)
{
    Vector3d< U > result(v);
    result.x.value *= scalar.value;
    result.y.value *= scalar.value;
    result.z.value *= scalar.value;

    return result;
}

/**
 * @brief Overloaded subtraction operator for Vector3d.
 * @tparam U The scalar type of the left-hand side vector.
 * @tparam T The scalar type of the right-hand side vector.
 * @param v The left-hand side vector.
 * @param scalar The scalar.
 * @return A new Vector3d representing the difference.
 */
template< typename U, typename T >
Vector3d< U > operator-(const Vector3d< U >& v, const T& scalar)
{
    Vector3d< U > result(v);
    result.x.value -= scalar.value;
    result.y.value -= scalar.value;
    result.z.value -= scalar.value;

    return result;
}

/**
 * @brief Overloaded division operator for Vector3d.
 * @tparam U The scalar type of the left-hand side vector.
 * @tparam T The scalar type of the right-hand side vector.
 * @param v The left-hand side vector.
 * @param scalar The scalar.
 * @return A new Vector3d representing the quotient.
 */
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
