#include "transformation.h"
#include "utils.h"
#include <string.h>

void Transformation::setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale)
{
    modelMatrix = T(translate) * Rz(rotate.z) * Ry(rotate.y) * Rx(rotate.x) * S(scale);
    inverse = modelMatrix.getInverse();
    inverseTranspose = inverse.getTranspose();
}

Matrix4 Transformation::T(const Vector3 &v)
{
    Matrix4 m;
    m[0][3] = v.x;
    m[1][3] = v.y;
    m[2][3] = v.z;
    return m;
}

Matrix4 Transformation::S(const Vector3 &v)
{
    Matrix4 m;
    m[0][0] = v.x;
    m[1][1] = v.y;
    m[2][2] = v.z;
    return m;
}

Matrix4 Transformation::Rx(float angle)
{
    Matrix4 m;
    m[1][1] = cos(angle);
    m[1][2] = -sin(angle);
    m[2][1] = sin(angle);
    m[2][2] = cos(angle);
    return m;
}

Matrix4 Transformation::Ry(float angle)
{
    Matrix4 m;
    m[0][0] = cos(angle);
    m[0][2] = sin(angle);
    m[2][0] = -sin(angle);
    m[2][2] = cos(angle);
    return m;
}

Matrix4 Transformation::Rz(float angle)
{
    Matrix4 m;
    m[0][0] = cos(angle);
    m[0][1] = -sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
    return m;
}
