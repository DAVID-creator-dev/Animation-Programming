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
    return Quaternion(
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w,
        w * q.w - x * q.x - y * q.y - z * q.z
    );
}

Vec3 Quaternion::multiplyVector(const Vec3& other) const
{
    Quaternion qV = Quaternion(other.x, other.y, other.z, 0);
    Quaternion qConjugate = Quaternion(-x, -y, -z, w); 

    Quaternion result = (*this) * qV * qConjugate;
   
    return Vec3(result.x, result.y, result.z);
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
    return Quaternion(w * other.x + x * other.w + y * other.z - z * other.y,
                      w * other.y - x * other.z + y * other.w + z * other.x,
                      w * other.z + x * other.y - y * other.x + z * other.w,
                      w * other.w - x * other.x - y * other.y - z * other.z); 
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

Mat4 Quaternion::Matrix() const
{
    float xx = x * x, yy = y * y, zz = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    Mat4 mat = mat.Identity(); 

    mat.data[0] = 1.0f - 2.0f * (yy + zz);
    mat.data[1] = 2.0f * (xy - wz);
    mat.data[2] = 2.0f * (xz + wy);

    mat.data[4] = 2.0f * (xy + wz);
    mat.data[5] = 1.0f - 2.0f * (xx + zz);
    mat.data[6] = 2.0f * (yz - wx);

    mat.data[8] = 2.0f * (xz - wy);
    mat.data[9] = 2.0f * (yz + wx);
    mat.data[10] = 1.0f - 2.0f * (xx + yy);

    return mat;
}
