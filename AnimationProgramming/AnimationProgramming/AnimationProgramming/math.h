#pragma once

class Vec3
{
public:
    float x, y, z;
    Vec3(float _x, float  _y, float _z) : x(_x), y(_y), z(_z) {}

    void print();
    Vec3 inverseVector() const;
    float norm() const;
    Vec3 additionVector(Vec3 v1, Vec3 v2) const;
    Vec3 addScalar(float scalar) const;
    Vec3 multiplyVectors(Vec3 v) const;
    Vec3 multiplyByScalar(float scalar) const;
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator+(float scalar) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float scalar) const;
    Vec3 operator*(const Vec3& other) const;
    Vec3 operator/(float scalar) const;
    Vec3& operator+=(const Vec3& other);
    Vec3& operator-=(const Vec3& other);
    Vec3& operator*=(float scalar);
    Vec3& operator*=(const Vec3& other);
    Vec3& operator/=(float scalar);

    static float Clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    static Vec3 Lerp(const Vec3& a, const Vec3& b, float t) {
        t = Clamp(t, 0.0f, 1.0f);
        return Vec3(a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t);
    }
};

class Quaternion
{
public:
    float x, y, z, w;
    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    void print();
    Quaternion inverseQuaternion() const;
    Quaternion multiplyQuaternion(Quaternion q) const;
    Vec3 multiplyVector(const Vec3& other) const;
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator*(float scalar) const;
    Quaternion& operator*=(const Quaternion& other);
    Quaternion& operator*=(float scalar);
};

