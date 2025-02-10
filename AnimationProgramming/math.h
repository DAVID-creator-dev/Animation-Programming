#pragma once
#include "iostream"

class Mat4; 
class Quaternion; 

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

    static Vec3 Lerp(const Vec3& a, const Vec3& b, float t);
};

class Quaternion
{
public:
    float x, y, z, w;
    Quaternion(float _x, float _y, float _z, float _w) :  x(_x), y(_y), z(_z), w(_w) {}

    void print();
    Mat4 Matrix() const;
    Quaternion inverseQuaternion() const;
    Quaternion multiplyQuaternion(Quaternion q) const;
    Vec3 multiplyVector(const Vec3& other) const;
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator*(float scalar) const;
    Quaternion& operator*=(const Quaternion& other);
    Quaternion& operator*=(float scalar);
};

class Mat3 {
public:
    Mat3() = default; 

    Mat3(float e00, float e01, float e02,
        float e10, float e11, float e12,
        float e20, float e21, float e22,
        float e30, float e31, float e32) 
    {
        data[0] = e00;  data[1] = e01;  data[2] = e02;  
        data[3] = e10;  data[4] = e11;  data[5] = e12;
        data[6] = e20;  data[7] = e21;  data[8] = e22; 
    }

    float data[9]; 

    float determinant() const;
};

class Mat4 {
public:
    Mat4(float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33)
    {
        data[0] = e00;  data[1] = e01;  data[2] = e02;  data[3] = e03;
        data[4] = e10;  data[5] = e11;  data[6] = e12;  data[7] = e13;
        data[8] = e20;  data[9] = e21;  data[10] = e22; data[11] = e23;
        data[12] = e30; data[13] = e31; data[14] = e32; data[15] = e33;
    }

    Mat4() {
        for (int i = 0; i < 16; i++)
            data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    float data[16]; 

    void Print(); 
    Mat4 Identity(); 
    void TRS(const Vec3& position, const Quaternion& rotation);
    void TransposeMatrix(); 
    Mat4 getCofactor() const;
    float determinant() const;
    double Determinant3x3(double a, double b, double c, double d, double e, double f, double g, double h, double i) const; 
    Mat4 getInverse() const; 

    void MultiplyMatrices(const Mat4& other); 
    Mat4 operator*(const Mat4& other) const; 
};
