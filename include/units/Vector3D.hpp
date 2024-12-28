#pragma once

#include "units/Angle.hpp"

namespace units {
/**
 * @class Vector3D
 *
 * @brief a 3D vector with x, y, and z components of a given quantity type.
 *
 * @tparam T the type of quantity to use for the vector components
 */
template <isQuantity T> class Vector3D {
    public:
        T x; /** x component */
        T y; /** y component */
        T z; /** z component */

        /**
         * @brief Construct a new Vector2D object
         *
         * This constructor initializes x, y, and z to 0
         */
        constexpr Vector3D() : x(0.0), y(0.0), z(0.0) {}

        /**
         * @brief Construct a new Vector2D object
         *
         * This constructor initializes x, y, and z to the given values
         *
         * @param nx x component
         * @param ny y component
         * @param nz z component
         */
        constexpr Vector3D(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}

        /**
         * @brief Create a new Vector3D object from spherical coordinates
         *
         * This constructor takes polar coordinates and converts them to cartesian coordinates
         *
         * @param t angle
         * @param m magnitude
         */
        constexpr static Vector3D fromPolar(const Vector3D<Angle>& t, T m) {
            m = m.abs();
            return Vector3D<T>(m * cos(t.x), m * cos(t.y), m * cos(t.z));
        }

        /**
         * @brief Create a new Vector3D object from an angle with a magnitude of 1
         *
         * @param t angle
         * @return Vector3D
         */
        constexpr static Vector3D unitVector(const Vector3D<Angle>& t) { return fromPolar(t, (T)1.0); }

        /**
         * @brief + operator overload
         *
         * This operator adds the x, y, and z components of two vectors
         * {a, b, c} + {d, e, f} = {a + d, b + e, c + f}
         *
         * @param other vector to add
         * @return Vector3D<T>
         */
        constexpr Vector3D<T> operator+(const Vector3D<T>& other) const {
            return Vector3D<T>(x + other.x, y + other.y, z + other.z);
        }

        /**
         * @brief - operator overload
         *
         * This operator subtracts the x, y, and z components of two vectors
         * {a, b, c} - {d, e, f} = {a - d, b - e, c - f}
         *
         * @param other vector to subtract
         * @return Vector3D<T>
         */
        constexpr Vector3D<T> operator-(const Vector3D<T>& other) const {
            return Vector3D<T>(x - other.x, y - other.y, z - other.z);
        }

        /**
         * @brief * operator overload
         *
         * This operator multiplies the x, y, and z components of a vector by a scalar
         * a * {b, c, d} = {a * b, a * c, a * d}
         *
         * @param factor scalar to multiply by
         * @return Vector3D<T>
         */
        constexpr Vector3D<T> operator*(double factor) const { return Vector3D<T>(x * factor, y * factor, z * factor); }

        /**
         * @brief / operator overload
         *
         * This operator divides the x, y, and z components of a vector by a scalar
         * {a, b, c} / d = {a / d, b / d, c / d}
         *
         * @param factor scalar to divide by
         * @return Vector3D<T>
         */
        constexpr Vector3D<T> operator/(double factor) const { return Vector3D<T>(x / factor, y / factor, z / factor); }

        /**
         * @brief += operator overload
         *
         * This operator adds the x, y, and z components of two vectors and stores the result in the calling vector
         * {a, b, c} += {d, e, f} => {a + d, b + e, c + f}
         *
         * @param other vector to add
         * @return Vector3D<T>&
         */
        constexpr Vector3D<T>& operator+=(const Vector3D<T>& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return (*this);
        }

        /**
         * @brief -= operator overload
         *
         * This operator subtracts the x, y, and z components of two vectors and stores the result in the calling vector
         * {a, b, c} -= {d, e, f} => {a - d, b - e, c - f}
         *
         * @param other vector to subtract
         * @return Vector3D<T>&
         */
        constexpr Vector3D<T>& operator-=(const Vector3D<T>& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return (*this);
        }

        /**
         * @brief *= operator overload
         *
         * This operator multiplies the x, y, and z components of a vector by a scalar and stores the result in the
         * calling vector
         * a *= {b, c, d} => {a * b, a * c, a * d}
         *
         * @param factor scalar to multiply by
         * @return Vector3D<T>&
         */
        constexpr Vector3D<T>& operator*=(double factor) {
            x *= factor;
            y *= factor;
            z *= factor;
            return (*this);
        }

        /**
         * @brief /= operator overload
         *
         * This operator divides the x, y, and z components of a vector by a scalar and stores the result in the
         * calling vector
         * {a, b, c} /= d => {a / d, b / d, c / d}
         *
         * @param factor scalar to divide by
         * @return Vector3D<T>&
         */
        constexpr Vector3D<T>& operator/=(double factor) {
            x /= factor;
            y /= factor;
            z /= factor;
            return (*this);
        }

        /**
         * @brief dot product of 2 Vector3D objects
         *
         * This function calculates the dot product of two vectors
         * a.dot(b) = (a.x * b.x) + (a.y * b.y) + (a.z * b.z)
         *
         * @tparam Q the type of quantity to use for the other vector
         * @tparam R the type of quantity to use for the result
         * @param other the vector to calculate the dot product with
         * @return R the dot product
         */
        template <isQuantity Q, isQuantity R = Multiplied<T, Q>> constexpr R dot(const Vector3D<Q>& other) const {
            return (x * other.x) + (y * other.y) + (z * other.z);
        }

        /**
         * @brief cross product of 2 Vector3D objects
         *
         * This function calculates the cross product of two vectors
         * a.cross(b) = { a.y * b.z - a.z * b.y,
         *                a.z * b.x - a.x * b.z,
         *                a.x * b.y - a.y * b.x }
         *
         * @tparam Q the type of quantity to use for the other vector
         * @tparam R the type of quantity to use for the result
         * @param other the vector to calculate the cross product with
         * @return Vector3D<R> the cross product
         */
        template <isQuantity Q, isQuantity R = Multiplied<T, Q>> constexpr Vector3D<R> cross(const Vector3D<Q>& other) const {
            return Vector3D<R>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
        }

        /**
         * @brief angle of the vector
         *
         * @return Angle
         */
        constexpr Vector3D<Angle> theta() const {
            const T mag = magnitude();
            return Vector3D<Angle>(acos(x / mag), acos(y / mag), acos(z / mag));
        }

        /**
         * @brief magnitude of the vector
         *
         * @return T
         */
        constexpr T magnitude() const { return sqrt(square(x) + square(y) + square(z)); }

        /**
         * @brief difference between two vectors
         *
         * TODO: figure out if this is even necessary, you could just use the - operator overload
         *
         * This function calculates the difference between two vectors
         * a.vectorTo(b) = {b.x - a.x, b.y - a.y, b.z - a.z}
         *
         * @param other the other vector
         * @return Vector3D<T>
         */
        constexpr Vector3D<T> vectorTo(const Vector3D<T>& other) const {
            return Vector2D<T>(other.x - x, other.y - y, other.z - z);
        }

        /**
         * @brief the angle between two vectors
         *
         * @param other the other vector
         * @return Angle
         */
        constexpr Angle angleTo(const Vector3D<T>& other) const {
            return units::acos(dot(other) / (magnitude() * other.magnitude()));
        }

        /**
         * @brief get the distance between two vectors
         *
         * @param other the other vector
         * @return T
         */
        constexpr T distanceTo(const Vector3D<T>& other) const { return vectorTo(other).magnitude(); }

        /**
         * @brief normalize the vector
         *
         * This function normalizes the vector, making it a unit vector
         *
         * @return Vector3D<T>
         */
        constexpr Vector3D<T> normalize() {
            T m = magnitude();
            return Vector2D<T>(x / m, y / m, z / m);
        }

        /**
         * @brief rotate the vector by an angle
         *
         * @param angle
         */
        constexpr void rotateBy(const Vector3D<Angle>& angle) {
            const T m = magnitude();
            const Vector3D<Angle> t = theta() + angle;
            x = m * cos(t.x);
            y = m * cos(t.y);
            z = m * cos(t.z);
        }

        /**
         * @brief rotate the vector to an angle
         *
         * @param angle
         */
        constexpr void rotateTo(const Vector3D<Angle>& angle) {
            const T m = magnitude();
            x = m * cos(angle.x);
            y = m * cos(angle.y);
            z = m * cos(angle.z);
        }

        /**
         * @brief get a copy of this vector rotated by an angle
         *
         * @param angle
         * @return Vector3D<T>
         */
        constexpr Vector3D<T> rotatedBy(const Vector3D<Angle>& angle) const {
            T m = magnitude();
            Angle t = theta() + angle;
            return fromPolar(t, m);
        }

        /**
         * @brief get a copy of this vector rotated to an angle
         *
         * @param angle
         * @return Vector3D<T>
         */
        constexpr Vector3D<T> rotatedTo(Angle angle) const {
            T m = magnitude();
            return fromPolar(angle, m);
        }
};

// define some common vector types
typedef Vector3D<Length> V3Position;
typedef Vector3D<LinearVelocity> V3Velocity;
typedef Vector3D<LinearAcceleration> V3Acceleration;
typedef Vector3D<Force> V3Force;
} // namespace units