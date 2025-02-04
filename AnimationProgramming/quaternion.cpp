#include <cstdio>

#include "math.h" 

void Quaternion::print()
{
    printf("(%.2f, %.2f, %.2f, %.2f)\n", x, y, z, w);
}

Quaternion Quaternion::inverseQuaternion() const
{
    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::multiplyQuaternion(Quaternion q) const
{
    return Quaternion(x * q.x - y * q.y - z * q.z - w * q.w,
                     y * q.x + x * q.y + z * q.w - w * q.z,
                     z * q.x + x * q.z - y * q.w - w * q.y,
                     x * q.w + y * q.z + z * q.y - w * q.x);
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
    return Quaternion(x * other.x - y * other.y - z * other.z - w * other.w,
                     y * other.x + x * other.y + z * other.w - w * other.z,
                     z * other.x + x * other.z - y * other.w - w * other.y,
                     x * other.w + y * other.z + z * other.y - w * other.x);
}

Quaternion Quaternion::operator*(float scalar) const
{
    return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
    float oldX = x;
    float oldY = y;
    float oldZ = z;
    float oldW = w;

    x = oldX * other.w + oldW * other.x + oldY * other.z - oldZ * other.y;
    y = oldY * other.w + oldW * other.y + oldZ * other.x - oldX * other.z;
    z = oldZ * other.w + oldW * other.z + oldX * other.y - oldY * other.x;
    w = oldW * other.w - oldX * other.x - oldY * other.y - oldZ * other.z;

    return *this;
}

Quaternion& Quaternion::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this; 
}