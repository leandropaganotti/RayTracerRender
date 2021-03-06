#pragma once

#include <iostream>
#include "vector.h"
#include "ray.h"
#include "aabb.h"

class Matrix4
{
public:
    Matrix4();

    Matrix4(float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33);

    inline float*       operator [] (uint8_t i)        { return mat[i]; }
    inline const float* operator [] (uint8_t i) const  { return mat[i]; }

    inline Matrix4 operator * (const Matrix4& M)   const;
    inline Vector3 operator * (const Vector3& P)   const;
    inline Ray     operator * (const Ray& ray)     const;
    inline AABB    operator * (const AABB& aabb)   const;


    inline Vector3 multiplyVector(const Vector3& V) const;

    inline Matrix4 getTranspose()  const;
    inline Matrix4 getInverse()    const;
    inline float   determinant()   const;

    friend std::ostream& operator << (std::ostream &os, Matrix4 m);

private:
    float mat[4][4];

    float det3(float a00, float a, float c, float d, float e, float f, float g, float h, float i) const;
};

inline
Matrix4::Matrix4():
    mat{{1,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}}
{

}

inline
Matrix4::Matrix4(float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13, float a20, float a21, float a22, float a23, float a30, float a31, float a32, float a33)
{
    mat[0][0] = a00, mat[0][1] = a01, mat[0][2] = a02, mat[0][3] = a03;
    mat[1][0] = a10, mat[1][1] = a11, mat[1][2] = a12, mat[1][3] = a13;
    mat[2][0] = a20, mat[2][1] = a21, mat[2][2] = a22, mat[2][3] = a23;
    mat[3][0] = a30, mat[3][1] = a31, mat[3][2] = a32, mat[3][3] = a33;
}

inline
Matrix4 Matrix4::operator *(const Matrix4 &M) const
{
    Matrix4 R;
    for (uint8_t i=0; i < 4; ++i)
        for (uint8_t j=0; j < 4; ++j)
        {
            R[i][j]  = mat[i][0] * M[0][j] + mat[i][1] * M[1][j] + mat[i][2] * M[2][j] + mat[i][3] * M[3][j];
        }
    return R;
}

inline
Vector3 Matrix4::operator *(const Vector3 &P) const
{
    Vector3 R;
    R[0] = mat[0][0] * P[0] + mat[0][1] * P[1] + mat[0][2] * P[2] + mat[0][3];
    R[1] = mat[1][0] * P[0] + mat[1][1] * P[1] + mat[1][2] * P[2] + mat[1][3];
    R[2] = mat[2][0] * P[0] + mat[2][1] * P[1] + mat[2][2] * P[2] + mat[2][3];
    return R;
}

inline
Ray Matrix4::operator *(const Ray &ray) const
{
    return Ray((*this) * ray.origin, multiplyVector(ray.direction), ray.tmax);
}

inline
AABB Matrix4::operator * (const AABB& aabb) const
{
    std::vector<Vector3> corners(8);
    Vector3 min = aabb.getMin(), max = aabb.getMax();
    corners[0] = (*this) * min;
    corners[1] = (*this) * Vector3(min.x, min.y, max.z);
    corners[2] = (*this) * Vector3(min.x, max.y, min.z);
    corners[3] = (*this) * Vector3(max.x, min.y, min.z);
    corners[4] = (*this) * Vector3(min.x, max.y, max.z);
    corners[5] = (*this) * Vector3(max.x, min.y, max.z);
    corners[6] = (*this) * Vector3(max.x, max.y, min.z);
    corners[7] = (*this) * max;

    return AABB(corners);
}

inline
Vector3 Matrix4::multiplyVector(const Vector3 &V) const
{
    Vector3 R;
    R[0] = mat[0][0] * V[0] + mat[0][1] * V[1] + mat[0][2] * V[2];
    R[1] = mat[1][0] * V[0] + mat[1][1] * V[1] + mat[1][2] * V[2];
    R[2] = mat[2][0] * V[0] + mat[2][1] * V[1] + mat[2][2] * V[2];
    return R;
}

inline
Matrix4 Matrix4::getTranspose() const
{
    return Matrix4(  mat[0][0], mat[1][0], mat [2][0], mat[3][0],
            mat[0][1], mat[1][1], mat [2][1], mat[3][1],
            mat[0][2], mat[1][2], mat [2][2], mat[3][2],
            mat[0][3], mat[1][3], mat [2][3], mat[3][3]
            );
}

inline
Matrix4 Matrix4::getInverse() const
{
    float det = determinant();

    Matrix4 inverse;
    inverse[0][0] =  det3(  mat[1][1], mat[1][2], mat[1][3],
                            mat[2][1], mat[2][2], mat[2][3],
                            mat[3][1], mat[3][2], mat[3][3]) / det;
    inverse[0][1] = -det3(  mat[0][1], mat[0][2], mat[0][3],
                            mat[2][1], mat[2][2], mat[2][3],
                            mat[3][1], mat[3][2], mat[3][3]) / det;
    inverse[0][2] =  det3(  mat[0][1], mat[0][2], mat[0][3],
                            mat[1][1], mat[1][2], mat[1][3],
                            mat[3][1], mat[3][2], mat[3][3]) / det;
    inverse[0][3] = -det3(  mat[0][1], mat[0][2], mat[0][3],
                            mat[1][1], mat[1][2], mat[1][3],
                            mat[2][1], mat[2][2], mat[2][3]) / det;

    inverse[1][0] = -det3(  mat[1][0], mat[1][2], mat[1][3],
                            mat[2][0], mat[2][2], mat[2][3],
                            mat[3][0], mat[3][2], mat[3][3]) / det;
    inverse[1][1] =  det3(  mat[0][0], mat[0][2], mat[0][3],
                            mat[2][0], mat[2][2], mat[2][3],
                            mat[3][0], mat[3][2], mat[3][3]) / det;
    inverse[1][2] = -det3(  mat[0][0], mat[0][2], mat[0][3],
                            mat[1][0], mat[1][2], mat[1][3],
                            mat[3][0], mat[3][2], mat[3][3]) / det;
    inverse[1][3] =  det3(  mat[0][0], mat[0][2], mat[0][3],
                            mat[1][0], mat[1][2], mat[1][3],
                            mat[2][0], mat[2][2], mat[2][3]) / det;

    inverse[2][0] =  det3(  mat[1][0], mat[1][1], mat[1][3],
                            mat[2][0], mat[2][1], mat[2][3],
                            mat[3][0], mat[3][1], mat[3][3]) / det;
    inverse[2][1] = -det3(  mat[0][0], mat[0][1], mat[0][3],
                            mat[2][0], mat[2][1], mat[2][3],
                            mat[3][0], mat[3][1], mat[3][3]) / det;
    inverse[2][2] =  det3(  mat[0][0], mat[0][1], mat[0][3],
                            mat[1][0], mat[1][1], mat[1][3],
                            mat[3][0], mat[3][1], mat[3][3]) / det;
    inverse[2][3] = -det3(  mat[0][0], mat[0][1], mat[0][3],
                            mat[1][0], mat[1][1], mat[1][3],
                            mat[2][0], mat[2][1], mat[2][3]) / det;

    inverse[3][0] = -det3(  mat[1][0], mat[1][1], mat[1][2],
                            mat[2][0], mat[2][1], mat[2][2],
                            mat[3][0], mat[3][1], mat[3][2]) / det;
    inverse[3][1] =  det3(  mat[0][0], mat[0][1], mat[0][2],
                            mat[2][0], mat[2][1], mat[2][2],
                            mat[3][0], mat[3][1], mat[3][2]) / det;
    inverse[3][2] = -det3(  mat[0][0], mat[0][1], mat[0][2],
                            mat[1][0], mat[1][1], mat[1][2],
                            mat[3][0], mat[3][1], mat[3][2]) / det;
    inverse[3][3] =  det3(  mat[0][0], mat[0][1], mat[0][2],
                            mat[1][0], mat[1][1], mat[1][2],
                            mat[2][0], mat[2][1], mat[2][2]) / det;

    return inverse;
}

inline
float Matrix4::determinant() const
{
    float det;
    det  = mat[0][0] * det3(mat[1][1], mat[1][2], mat[1][3],
                            mat[2][1], mat[2][2], mat[2][3],
                            mat[3][1], mat[3][2], mat[3][3]);

    det -= mat[0][1] * det3(mat[1][0], mat[1][2], mat[1][3],
                            mat[2][0], mat[2][2], mat[2][3],
                            mat[3][0], mat[3][2], mat[3][3]);

    det += mat[0][2] * det3(mat[1][0], mat[1][1], mat[1][3],
                            mat[2][0], mat[2][1], mat[2][3],
                            mat[3][0], mat[3][1], mat[3][3]);

    det -= mat[0][3] * det3(mat[1][0], mat[1][1], mat[1][2],
                            mat[2][0], mat[2][1], mat[2][2],
                            mat[3][0], mat[3][1], mat[3][2]);
    return det;
}

inline
std::ostream& operator <<(std::ostream &os, Matrix4 m)
{
    size_t i;
    for ( i=0; i < 3; ++i)
        os << "[" << m.mat[i][0] << " " << m.mat[i][1]<< " " << m.mat[i][2] << " " << m.mat[i][3] << "] " << std::endl;
    os << "[" << m.mat[i][0] << " " << m.mat[i][1]<< " " << m.mat[i][2] << " " << m.mat[i][3] << "] ";
    return os;
}

inline
float Matrix4::det3(float a, float b, float c, float d, float e, float f, float g, float h, float i) const
{
    return a*e*i + d*h*c + g*b*f - g*e*c - d*b*i -a*h*f;
}
