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

float Vec3::Clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Vec3 Vec3::Lerp(const Vec3& a, const Vec3& b, float t)
{
    t = Clamp(t, 0.0f, 1.0f);
    return Vec3(a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t);
}
