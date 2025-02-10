#include "math.h"

#include <valarray>

void Vec3::print()
{
    printf("(%.2f, %.2f, %.2f)\n", x, y, z);
}

Vec3 Vec3::inverseVector() const
{
    return Vec3(-x, -y, -z);
}

float Vec3::norm() const
{
    return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::additionVector(Vec3 v1, Vec3 v2) const
{
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3 Vec3::addScalar(float scalar) const
{
    return Vec3(x + scalar, y + scalar, z + scalar);
}

Vec3 Vec3::multiplyVectors(Vec3 v) const
{
    return Vec3(x * v.x, y * v.y, z * v.z);
}

Vec3 Vec3::multiplyByScalar(float scalar) const
{
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator+(float scalar) const
{
    return Vec3(x + scalar, y + scalar, z + scalar);
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(float scalar) const
{
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator*(const Vec3& other) const
{
    return Vec3(x * other.x, y * other.y, z * other.z);
}

Vec3 Vec3::operator/(float scalar) const
{
    return Vec3(x / scalar, y / scalar, z / scalar);
}

Vec3& Vec3::operator+=(const Vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3& Vec3::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vec3& Vec3::operator*=(const Vec3& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

Vec3& Vec3::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}