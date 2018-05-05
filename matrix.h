#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

#include "vector.h"

template<typename T>
struct Matrix4x4
{
    T mat[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

    Matrix4x4(){}

    Matrix4x4(  T a00, T a01, T a02, T a03,
                T a10, T a11, T a12, T a13,
                T a20, T a21, T a22, T a23,
                T a30, T a31, T a32, T a33)
    {
        mat[0][0] = a00, mat[0][1] = a01, mat[0][2] = a02, mat[0][3] = a03;
        mat[1][0] = a10, mat[1][1] = a11, mat[1][2] = a12, mat[1][3] = a13;
        mat[2][0] = a20, mat[2][1] = a21, mat[2][2] = a22, mat[2][3] = a23;
        mat[3][0] = a30, mat[3][1] = a31, mat[3][2] = a32, mat[3][3] = a33;
    }

    T* operator [] (uint8_t i) { return mat[i]; }
    const T* operator [] (uint8_t i) const { return mat[i]; }

    friend Matrix4x4 operator * (const Matrix4x4& A, const Matrix4x4& B)
    {
        Matrix4x4 R;
        multiply_4x4_4x4(R.mat, A.mat, B.mat);
        return R;

    }

    friend Vector3<T> operator * (const Matrix4x4& M, const Vector3<T>& V)
    {
        Vector3<T> R;
        multiply_4x4_Vector(R.vec, M.mat, V.vec);
        return R;
    }

    friend std::ostream& operator << (std::ostream &os, Matrix4x4 m)
    {
        size_t i;
        for ( i=0; i < 3; ++i)
            os << "[" << m.mat[i][0] << " " << m.mat[i][1]<< " " << m.mat[i][2] << " " << m.mat[i][3] << "] " << std::endl;
        os << "[" << m.mat[i][0] << " " << m.mat[i][1]<< " " << m.mat[i][2] << " " << m.mat[i][3] << "] ";
        return os;
    }

private:
    // R = A * B
    static void multiply_4x4_4x4(T R[][4], const T A[][4], const T B[][4])
    {
        for (size_t i=0; i < 4; ++i)
            for (size_t j=0; j < 4; ++j)
                {
                    R[i][j]  = A[i][0] * B[0][j];
                    R[i][j] += A[i][1] * B[1][j];
                    R[i][j] += A[i][2] * B[2][j];
                    R[i][j] += A[i][3] * B[3][j];
                }
    }
    // R = M * V
    static void multiply_4x4_Vector(T R[], const T M[][4], const T V[])
    {
        R[0] = M[0][0] * V[0] + M[0][1] * V[1] + M[0][2] * V[2] + M[0][3];
        R[1] = M[1][0] * V[0] + M[1][1] * V[1] + M[1][2] * V[2] + M[1][3];
        R[2] = M[2][0] * V[0] + M[2][1] * V[1] + M[2][2] * V[2] + M[2][3];
//        T w   = M[3][0] * V[0] + M[3][1] * V[1] + M[3][2] * V[2] + M[3][3];
//        if ( w != 1 && w != 0) {
//            RV[0] /= w, RV[1] /= w, RV[2] /= w;
//        }
    }

};

typedef Matrix4x4<float> Matrix4x4f;

#endif // MATRIX_H
