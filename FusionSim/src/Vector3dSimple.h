#ifndef VECTOR3D_SIMPLE_H
#define VECTOR3D_SIMPLE_H

#include <cmath>
#include <iostream>

namespace fusion
{
    struct Vector3d
    {
        double x, y, z;

        Vector3d() : x(0), y(0), z(0) {}
        Vector3d(const double px, const double py, const double pz) : x(px), y(py), z(pz) {}
        Vector3d(const Vector3d& rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

        [[nodiscard]] double magnitude() const
        {
            return std::sqrt(x*x + y*y + z*z);
        }

        [[nodiscard]] double norm() const
        {
            return magnitude();
        }

        [[nodiscard]]double squaredNorm() const
        {
            return x*x + y*y + z*z;
        }

        [[nodiscard]] Vector3d normalized() const
        {
            const double mag = magnitude();
            if (mag > 0)
            {
                return Vector3d(x/mag, y/mag, z/mag);
            }
            return Vector3d(0, 0, 0);
        }

        [[nodiscard]] Vector3d cross(const Vector3d& rhs) const
        {
            return Vector3d(
                y * rhs.z - z * rhs.y,
                z * rhs.x - x * rhs.z,
                x * rhs.y - y * rhs.x
            );
        }

        [[nodiscard]]double dot(const Vector3d& rhs) const
        {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        Vector3d operator+(const Vector3d& rhs) const
        {
            return Vector3d(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        Vector3d operator-(const Vector3d& rhs) const
        {
            return Vector3d(x - rhs.x, y - rhs.y, z - rhs.z);
        }


        Vector3d operator-() const
        {
            return Vector3d(-x, -y, -z);
        }

        Vector3d operator*(const double scalar) const
        {
            return Vector3d(x * scalar, y * scalar, z * scalar);
        }

        Vector3d operator/(const double scalar) const
        {
            return Vector3d(x / scalar, y / scalar, z / scalar);
        }

        Vector3d& operator+=(const Vector3d& rhs)
        {
            x += rhs.x; y += rhs.y; z += rhs.z;
            return *this;
        }

        Vector3d& operator-=(const Vector3d& rhs)
        {
            x -= rhs.x; y -= rhs.y; z -= rhs.z;
            return *this;
        }

        Vector3d& operator*=(const double scalar)
        {
            x *= scalar; y *= scalar; z *= scalar;
            return *this;
        }

        Vector3d& operator/=(const double scalar)
        {
            x /= scalar; y /= scalar; z /= scalar;
            return *this;
        }

        friend Vector3d operator*(const double scalar, const Vector3d& v)
        {
            return v * scalar;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector3d& v)
        {
            os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
            return os;
        }
    };
}

#endif // VECTOR3D_SIMPLE_H