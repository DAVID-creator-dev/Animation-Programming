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
    Mat4 translationMatrix = translationMatrix.SetTranslation(position); 
    Mat4 scaleMatrix = Mat4::Identity(); 

    Mat4 result = translationMatrix * rotationMatrix * scaleMatrix; 

    for (int i = 0; i < 16; ++i) {
        data[i] = result.data[i];
    }
}

Mat4 Mat4::SetTranslation(const Vec3& position)
{
    Mat4 translation = translation.Identity();

    translation.data[3] = position.x;
    translation.data[7] = position.y;
    translation.data[11] = position.z;

    return translation; 
}

Mat4 Mat4::InvertMatrix()
{
    Mat4 result = result.Identity();
    double inv[16], det;
    int i;

    inv[0] = data[5] * data[10] * data[15] -
        data[5] * data[11] * data[14] -
        data[9] * data[6] * data[15] +
        data[9] * data[7] * data[14] +
        data[13] * data[6] * data[11] -
        data[13] * data[7] * data[10];

    inv[4] = -data[4] * data[10] * data[15] +
        data[4] * data[11] * data[14] +
        data[8] * data[6] * data[15] -
        data[8] * data[7] * data[14] -
        data[12] * data[6] * data[11] +
        data[12] * data[7] * data[10];

    inv[8] = data[4] * data[9] * data[15] -
        data[4] * data[11] * data[13] -
        data[8] * data[5] * data[15] +
        data[8] * data[7] * data[13] +
        data[12] * data[5] * data[11] -
        data[12] * data[7] * data[9];

    inv[12] = -data[4] * data[9] * data[14] +
        data[4] * data[10] * data[13] +
        data[8] * data[5] * data[14] -
        data[8] * data[6] * data[13] -
        data[12] * data[5] * data[10] +
        data[12] * data[6] * data[9];

    inv[1] = -data[1] * data[10] * data[15] +
        data[1] * data[11] * data[14] +
        data[9] * data[2] * data[15] -
        data[9] * data[3] * data[14] -
        data[13] * data[2] * data[11] +
        data[13] * data[3] * data[10];

    inv[5] = data[0] * data[10] * data[15] -
        data[0] * data[11] * data[14] -
        data[8] * data[2] * data[15] +
        data[8] * data[3] * data[14] +
        data[12] * data[2] * data[11] -
        data[12] * data[3] * data[10];

    inv[9] = -data[0] * data[9] * data[15] +
        data[0] * data[11] * data[13] +
        data[8] * data[1] * data[15] -
        data[8] * data[3] * data[13] -
        data[12] * data[1] * data[11] +
        data[12] * data[3] * data[9];

    inv[13] = data[0] * data[9] * data[14] -
        data[0] * data[10] * data[13] -
        data[8] * data[1] * data[14] +
        data[8] * data[2] * data[13] +
        data[12] * data[1] * data[10] -
        data[12] * data[2] * data[9];

    inv[2] = data[1] * data[6] * data[15] -
        data[1] * data[7] * data[14] -
        data[5] * data[2] * data[15] +
        data[5] * data[3] * data[14] +
        data[13] * data[2] * data[7] -
        data[13] * data[3] * data[6];

    inv[6] = -data[0] * data[6] * data[15] +
        data[0] * data[7] * data[14] +
        data[4] * data[2] * data[15] -
        data[4] * data[3] * data[14] -
        data[12] * data[2] * data[7] +
        data[12] * data[3] * data[6];

    inv[10] = data[0] * data[5] * data[15] -
        data[0] * data[7] * data[13] -
        data[4] * data[1] * data[15] +
        data[4] * data[3] * data[13] +
        data[12] * data[1] * data[7] -
        data[12] * data[3] * data[5];

    inv[14] = -data[0] * data[5] * data[14] +
        data[0] * data[6] * data[13] +
        data[4] * data[1] * data[14] -
        data[4] * data[2] * data[13] -
        data[12] * data[1] * data[6] +
        data[12] * data[2] * data[5];

    inv[3] = -data[1] * data[6] * data[11] +
        data[1] * data[7] * data[10] +
        data[5] * data[2] * data[11] -
        data[5] * data[3] * data[10] -
        data[9] * data[2] * data[7] +
        data[9] * data[3] * data[6];

    inv[7] = data[0] * data[6] * data[11] -
        data[0] * data[7] * data[10] -
        data[4] * data[2] * data[11] +
        data[4] * data[3] * data[10] +
        data[8] * data[2] * data[7] -
        data[8] * data[3] * data[6];

    inv[11] = -data[0] * data[5] * data[11] +
        data[0] * data[7] * data[9] +
        data[4] * data[1] * data[11] -
        data[4] * data[3] * data[9] -
        data[8] * data[1] * data[7] +
        data[8] * data[3] * data[5];

    inv[15] = data[0] * data[5] * data[10] -
        data[0] * data[6] * data[9] -
        data[4] * data[1] * data[10] +
        data[4] * data[2] * data[9] +
        data[8] * data[1] * data[6] -
        data[8] * data[2] * data[5];

    det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];

    if (det == 0) {
        std::cout << "non ?" << std::endl;
        return Identity(); 
    }

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        result.data[i] = inv[i] * det;

    return result; 
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
