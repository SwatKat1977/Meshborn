/*
Meshborn
Copyright (C) 2025 SwatKat1977

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef VECTOR3D_H_
#define VECTOR3D_H_
#include <cmath>
#include <stdexcept>
#include <ostream>

namespace Meshborn {

class Vector3D {
 public:
    float x, y, z;

    // Constructors
    Vector3D() : x(0.f), y(0.f), z(0.f) {}
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    /**
     * @brief Computes the Euclidean length (magnitude) of a 3D vector.
     *
     * @return The length of the vector.
     */
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    /**
     * @brief Normalises a 3D vector in-place to have a length of 1.
     *
     * If the vector has near-zero length, it is set to (0, 0, 0) to avoid
     * division by zero.
     */
    void Normalise() {
        float len = Length();
        if (len == 0.f)
            throw std::runtime_error("Cannot normalize zero-length vector");
        x /= len;
        y /= len;
        z /= len;
    }

    // Return a normalized copy of this vector
    Vector3D Normalized() const {
        float len = Length();
        if (len == 0.f)
            throw std::runtime_error("Cannot normalize zero-length vector");
        return { x / len, y / len, z / len };
    }

    // Dot product
    float Dot(const Vector3D& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    /**
     * @brief Computes the cross product of two 3D vectors.
     *
     * @param other The second vector.
     * @return A new Vector3D representing the cross product a × b.
     */
    Vector3D Cross(const Vector3D& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    // Operators
    Vector3D operator+(const Vector3D& rhs) const {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    Vector3D operator-(const Vector3D& rhs) const {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    Vector3D operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    Vector3D operator/(float scalar) const {
        if (scalar == 0.f)
            throw std::runtime_error("Division by zero");
        return { x / scalar, y / scalar, z / scalar };
    }

    bool operator==(const Vector3D& rhs) const {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
};

}   // namespace Meshborn

#endif  //  VECTOR3D_H_
