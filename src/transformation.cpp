#include "transformation.h"



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

Matrix4 Transformation::TSR(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale)
{
    return T(translate) * Rz(rotate.z) * Ry(rotate.y) * Rx(rotate.x) * S(scale);
}

Matrix4 Transformation::RotationDir(const Vector3 &direction)
{
    Matrix4 R;
//    Vector3 u,v=direction, w, n(1,0,0),m(0,1,0);
//    u = v%n; if(u.length()<0.01f)u = v%m;
//    w=u%v;
    const Vector3 n(1,0,0), m(0,1,0);

    Vector3 w = direction;
    w.normalize();
    Vector3 u = w%n; if(u.length()<0.01f)u = w%m;
    Vector3 v=w%u;

    R[0][0] = u.x;
    R[1][0] = u.y;
    R[2][0] = u.z;

    R[0][1] = v.x;
    R[1][1] = v.y;
    R[2][1] = v.z;

    R[0][2] = w.x;
    R[1][2] = w.y;
    R[2][2] = w.z;
    return R;
}
