#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

#include "vector.h"
#include "ray.h"

struct Matrix4
{
    float mat[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

    Matrix4() = default;

    Matrix4(float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33)
    {
        mat[0][0] = a00, mat[0][1] = a01, mat[0][2] = a02, mat[0][3] = a03;
        mat[1][0] = a10, mat[1][1] = a11, mat[1][2] = a12, mat[1][3] = a13;
        mat[2][0] = a20, mat[2][1] = a21, mat[2][2] = a22, mat[2][3] = a23;
        mat[3][0] = a30, mat[3][1] = a31, mat[3][2] = a32, mat[3][3] = a33;
    }

    float* operator [] (uint8_t i) { return mat[i]; }
    const float* operator [] (uint8_t i) const { return mat[i]; }

    Matrix4 operator * (const Matrix4& M) const
    {
        Matrix4 R;
        for (size_t i=0; i < 4; ++i)
            for (size_t j=0; j < 4; ++j)
                {
                    R[i][j]  = mat[i][0] * M[0][j];
                    R[i][j] += mat[i][1] * M[1][j];
                    R[i][j] += mat[i][2] * M[2][j];
                    R[i][j] += mat[i][3] * M[3][j];
                }
        return R;
    }

    Vector3 operator * (const Vector3& P) const
    {
        Vector3 R;
        R[0] = mat[0][0] * P[0] + mat[0][1] * P[1] + mat[0][2] * P[2] + mat[0][3];
        R[1] = mat[1][0] * P[0] + mat[1][1] * P[1] + mat[1][2] * P[2] + mat[1][3];
        R[2] = mat[2][0] * P[0] + mat[2][1] * P[1] + mat[2][2] * P[2] + mat[2][3];
        return R;
    }

    Ray operator * (const Ray& ray) const
    {
        Ray R;
        R.origin = (*this) * ray.origin;
        R.direction = ((*this) * ray.direction).normalize();
        return R;
    }

    Vector3 multiplyVector(const Vector3& V) const
    {
        Vector3 R;
        R[0] = mat[0][0] * V[0] + mat[0][1] * V[1] + mat[0][2] * V[2];
        R[1] = mat[1][0] * V[0] + mat[1][1] * V[1] + mat[1][2] * V[2];
        R[2] = mat[2][0] * V[0] + mat[2][1] * V[1] + mat[2][2] * V[2];
        return R;
    }

    Matrix4 transpose() const
    {
        return Matrix4(mat[0][0], mat[1][0], mat [2][0], mat[3][0],
                         mat[0][1], mat[1][1], mat [2][1], mat[3][1],
                         mat[0][2], mat[1][2], mat [2][2], mat[3][2],
                         mat[0][3], mat[1][3], mat [2][3], mat[3][3]
                        );
    }

    friend std::ostream& operator << (std::ostream &os, Matrix4 m)
    {
        size_t i;
        for ( i=0; i < 3; ++i)
            os << "[" << m.mat[i][0] << " " << m.mat[i][1]<< " " << m.mat[i][2] << " " << m.mat[i][3] << "] " << std::endl;
        os << "[" << m.mat[i][0] << " " << m.mat[i][1]<< " " << m.mat[i][2] << " " << m.mat[i][3] << "] ";
        return os;
    }
};

#endif // MATRIX_H
