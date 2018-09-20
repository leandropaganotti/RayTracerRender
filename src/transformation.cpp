#include "transformation.h"
#include "utils.h"
#include <string.h>

void Transformation::setTransformation(const Vector3f &translate, const Vector3f &rotate, const Vector3f &scale)
{
    Matrix4x4f src = T(translate) * Rz(rotate.z) * Ry(rotate.y) * Rx(rotate.x) * S(scale);
    memcpy(transformation.mat, src.mat, sizeof(float) * 16);
    inverse = S(1.0f/scale) * Rx(-rotate.x) * Ry(-rotate.y) * Rz(-rotate.z) * T(-translate);
    inverseTranspose = inverse.transpose();
}

Matrix4x4f Transformation::T(const Vector3f &v)
{
    Matrix4x4f m;
    m[0][3] = v.x;
    m[1][3] = v.y;
    m[2][3] = v.z;
    return m;
}

Matrix4x4f Transformation::S(const Vector3f &v)
{
    Matrix4x4f m;
    m[0][0] = v.x;
    m[1][1] = v.y;
    m[2][2] = v.z;
    return m;
}

Matrix4x4f Transformation::Rx(float angle)
{
    Matrix4x4f m;
    m[1][1] = cos(angle);
    m[1][2] = -sin(angle);
    m[2][1] = sin(angle);
    m[2][2] = cos(angle);
    return m;
}

Matrix4x4f Transformation::Ry(float angle)
{
    Matrix4x4f m;
    m[0][0] = cos(angle);
    m[0][2] = sin(angle);
    m[2][0] = -sin(angle);
    m[2][2] = cos(angle);
    return m;
}

Matrix4x4f Transformation::Rz(float angle)
{
    Matrix4x4f m;
    m[0][0] = cos(angle);
    m[0][1] = -sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
    return m;
}
