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

void Mat4::CreateTransformMatrix(const Vec3& position, const Quaternion& rotation)
{
    Mat4 rotationMatrix = rotation.Matrix(); 
    Mat4 transform = Mat4::Identity();

    transform.MultiplyMatrices(rotationMatrix);  
    transform.SetTranslation(position); 

    for (int i = 0; i < 16; ++i) {
        data[i] = transform.data[i];
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

void Mat4::SetTranslation(const Vec3& translation)
{
    data[12] = translation.x;
    data[13] = translation.y;
    data[14] = translation.z;
}
