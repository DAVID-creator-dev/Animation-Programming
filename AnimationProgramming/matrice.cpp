#include "math.h"

void Mat4::Print()
{
    for (int i = 0; i < 4; i++) {  
        for (int j = 0; j < 4; j++) {  
            std::cout << data[i * 4 + j] << "\t";  
        }
        std::cout << std::endl;  
    }
}

Mat4 Mat4::Identity() {
	return Mat4(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f); 
}

void Mat4::TRS(const Vec3& position, const Quaternion& rotation)
{
    Mat4 rotationMatrix = rotation.Matrix(); 
    Mat4 translationMatrix = Mat4::Identity();
    Mat4 scaleMatrix = Mat4::Identity(); 

    translationMatrix.data[3] = position.x;
    translationMatrix.data[7] = position.y;
    translationMatrix.data[11] = position.z;

    rotationMatrix.MultiplyMatrices(scaleMatrix);

    translationMatrix.MultiplyMatrices(rotationMatrix);  // * rotationMatrix * scaleMatrix; 

        Mat4 TRS = translationMatrix; 

    for (int i = 0; i < 16; ++i) {
        data[i] = TRS.data[i];
    }
}

void Mat4::MultiplyMatrices(const Mat4& other)
{
    float result[16]; 

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i * 4 + j] =
                data[i * 4 + 0] * other.data[0 * 4 + j] +
                data[i * 4 + 1] * other.data[1 * 4 + j] +
                data[i * 4 + 2] * other.data[2 * 4 + j] +
                data[i * 4 + 3] * other.data[3 * 4 + j];
        }
    }

    for (int i = 0; i < 16; ++i) {
        data[i] = result[i];
    }
}

Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 result;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.data[i * 4 + j] =
                data[i * 4 + 0] * other.data[0 * 4 + j] +
                data[i * 4 + 1] * other.data[1 * 4 + j] +
                data[i * 4 + 2] * other.data[2 * 4 + j] +
                data[i * 4 + 3] * other.data[3 * 4 + j];
        }
    }

    return result; 
}

Mat4 Mat4::getInverse() const {
    float det = determinant();
    if (det == 0) {
        std::cout << "Matrix is singular, cannot invert." << std::endl;
        exit(1);
    }

    Mat4 cofactorMatrix = getCofactor();
    cofactorMatrix.TransposeMatrix();

    double invDet = 1.0 / det;
    Mat4 inverseMatrix;
    for (size_t i = 0; i < 16; ++i) {
        inverseMatrix.data[i] = invDet * cofactorMatrix.data[i];
    }

    return inverseMatrix;
}

float Mat4::determinant() const {
    float det = 0;

    det += data[0] * Determinant3x3(data[5], data[6], data[7], data[9], data[10], data[11], data[13], data[14], data[15]);
    det -= data[1] * Determinant3x3(data[4], data[6], data[7], data[8], data[10], data[11], data[12], data[14], data[15]);
    det += data[2] * Determinant3x3(data[4], data[5], data[7], data[8], data[9], data[11], data[12], data[13], data[15]);
    det -= data[3] * Determinant3x3(data[4], data[5], data[6], data[8], data[9], data[10], data[12], data[13], data[14]);

    return det;
}

double Mat4::Determinant3x3(double a, double b, double c, double d, double e, double f, double g, double h, double i) const {
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

void Mat4::TransposeMatrix()
{
    double temp;
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            temp = data[i * 4 + j];
            data[i * 4 + j] = data[j * 4 + i];
            data[j * 4 + i] = temp;
        }
    }
}

Mat4 Mat4::getCofactor() const {
    Mat4 solution;

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            Mat3 subMatrix;
            int p = 0, q = 0;
            for (size_t x = 0; x < 4; ++x) {
                if (x == i) continue;
                q = 0;
                for (size_t y = 0; y < 4; ++y) {
                    if (y == j) continue;
                    subMatrix.data[p * 3 + q] = data[x * 4 + y];
                    ++q;
                }
                ++p;
            }
            double cofactorSign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
            solution.data[i * 4 + j] = cofactorSign * subMatrix.determinant();
        }
    }

    return solution;
}

float Mat3::determinant() const {
    float det = data[0] * (data[4] * data[8] - data[5] * data[7])
        - data[1] * (data[3] * data[8] - data[5] * data[6])
        + data[2] * (data[3] * data[7] - data[4] * data[6]);
    return det;
}
