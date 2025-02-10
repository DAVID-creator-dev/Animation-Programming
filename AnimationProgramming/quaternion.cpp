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

float Quaternion::Norm() const
{
    return sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::Normalize()
{
    float n = Norm();
    if (n > 0.0f) { 
        w /= n;
        x /= n;
        y /= n;
        z /= n;
    }

    return *this;
}

Vec3 Quaternion::multiplyVector(const Vec3& other) const
{
    Quaternion qV = Quaternion(other.x, other.y, other.z, 0);
    Quaternion qConjugate = Quaternion(-x, -y, -z, w); 

    Quaternion result = (*this) * qV * qConjugate;
   
    return Vec3(result.x, result.y, result.z);
}

Quaternion Quaternion::Slerp(const Quaternion& a, const Quaternion& b, float t)
{
    t = Clamp(t, 0.0f, 1.0f);

    float dot = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;

    if (dot > 0.9995f) {
        return Quaternion(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t, 
            a.w + (b.w - a.w) * t
        ).Normalize();
    }

    if (dot < 0.0f) {
        return Slerp(a, Quaternion(-b.x, -b.y, -b.z, -b.w), t);
    }

    float theta_0 = acos(dot);    
    float theta = theta_0 * t;    

    float sin_theta = sin(theta);
    float sin_theta_0 = sin(theta_0);

    float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;

    return Quaternion(
        (s0 * a.x) + (s1 * b.x),
        (s0 * a.y) + (s1 * b.y),
        (s0 * a.z) + (s1 * b.z), 
        (s0 * a.w) + (s1 * b.w)
    ).Normalize();
}

float Quaternion::Clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
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
    Mat4 mat = mat.Identity(); 

    mat.data[0] = 1.0f - 2.0f * (y * y + z * z);
    mat.data[1] = 2.0f * (x * y - z * w);
    mat.data[2] = 2.0f * (x * z + y * w);

    mat.data[4] = 2.0f * (x * y + z * w);
    mat.data[5] = 1.0f - 2.0f * (x * x + z * z);
    mat.data[6] = 2.0f * (y * z - x * w);

    mat.data[8] = 2.0f * (x * z - y * w);
    mat.data[9] = 2.0f * (y * z + x * w);
    mat.data[10] = 1.0f - 2.0f * (x * x + y * y);

    return mat;
}
