#ifndef VECTOR3D_SIMPLE_H
#define VECTOR3D_SIMPLE_H

#include <cmath>
#include <iostream>

/// @brief FusionSim - a simulator for FFR \namespace  fusion
namespace fusion
{
    /// @brief A struct to represet a 3D Vector. \struct Vector3d
    struct Vector3d
    {
        double x, y, z;

        /**
         * @brief Simple ctor.
         */
        Vector3d() : x(0), y(0), z(0) {}

        /**
         * @brief Ctor with axis.
         *
         * @param px
         * @param py
         * @param pz
         */
        Vector3d(const double px, const double py, const double pz) : x(px), y(py), z(pz) {}

        /**
         * @brief Ctor with 3D Vector.
         *
         * @param rhs
         */
        Vector3d(const Vector3d& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

        /**
         * @brief Simple getter for magnitude
         * @return A double representing the magnitude
         */
        [[nodiscard]] double magnitude() const
        {
            return std::sqrt(x*x + y*y + z*z);
        }
        [[nodiscard]] double norm() const
        {
            return magnitude();
        }

        /**
         * @brief Simple getter for the squared norm.
         * @return A double representing the squared norm.
         */
        [[nodiscard]]double squaredNorm() const
        {
            return x*x + y*y + z*z;
        }

        /**
         * @brief Getter for the normalized Vector.
         * @return A Normalized 3D Vector.
         */
        [[nodiscard]] Vector3d normalized() const
        {
            const double mag = magnitude();
            if (mag > 0)
            {
                return Vector3d(x/mag, y/mag, z/mag);
            }
            return Vector3d(0, 0, 0);
        }

        /**
         * @brief Getter for the cross.
         * @param rhs
         * @return A double represnting the cross.
         */
        [[nodiscard]] Vector3d cross(const Vector3d& rhs) const
        {
            return Vector3d(
                y * rhs.z - z * rhs.y,
                z * rhs.x - x * rhs.z,
                x * rhs.y - y * rhs.x
            );
        }

        /**
         * @brief Getter for the dot.
         * @param rhs
         * @return A double representing the dot.
         */
        [[nodiscard]]double dot(const Vector3d& rhs) const
        {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        /**
         * @brief Overloaded + operator.
         */
        Vector3d operator+(const Vector3d& rhs) const
        {
            return Vector3d(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        /**
         * @brief Overloaded - operator.
         */
        Vector3d operator-(const Vector3d& rhs) const
        {
            return Vector3d(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        /**
         * @brief Overloaded - operator.
         */
        Vector3d operator-() const
        {
            return Vector3d(-x, -y, -z);
        }

        /**
         * @brief Overloaded * operator.
         */
        Vector3d operator*(const double scalar) const
        {
            return Vector3d(x * scalar, y * scalar, z * scalar);
        }

        /**
         * @brief Overloaded / operator.
         */
        Vector3d operator/(const double scalar) const
        {
            return Vector3d(x / scalar, y / scalar, z / scalar);
        }

        /**
         * @brief Overloaded -= operator.
         */
        Vector3d& operator+=(const Vector3d& rhs)
        {
            x += rhs.x; y += rhs.y; z += rhs.z;
            return *this;
        }

        /**
         * @brief Overloaded -= operator.
         */
        Vector3d& operator-=(const Vector3d& rhs)
        {
            x -= rhs.x; y -= rhs.y; z -= rhs.z;
            return *this;
        }

        /**
         * @brief Overloaded *= operator.
         */
        Vector3d& operator*=(const double scalar)
        {
            x *= scalar; y *= scalar; z *= scalar;
            return *this;
        }

        /**
         * @brief Overloaded /= operator.
         */
        Vector3d& operator/=(const double scalar)
        {
            x /= scalar; y /= scalar; z /= scalar;
            return *this;
        }

        /**
         * @brief Overloaded * operator.
         */
        friend Vector3d operator*(const double scalar, const Vector3d& v)
        {
            return v * scalar;
        }

        /**
         * @brief Overloaded << operator.
         */
        friend std::ostream& operator<<(std::ostream& os, const Vector3d& v)
        {
            os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
            return os;
        }
    };
}

#endif // VECTOR3D_SIMPLE_H