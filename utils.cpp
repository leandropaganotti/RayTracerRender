#include "utils.h"

double ms_time()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return (double) currentTime.tv_sec * 1000.0 + (double)currentTime.tv_usec/1000.0;
}

float deg2rad(float deg)
{
    return deg * PI / 180.0;
}

float rad2deg(float rad)
{
    return rad * 180.0 / PI;
}

Matrix4x4f T(const Vector3f &v)
{
    Matrix4x4f m;
    m[0][3] = v.x;
    m[1][3] = v.y;
    m[2][3] = v.z;
    return m;
}

Matrix4x4f S(const Vector3f &v)
{
    Matrix4x4f m;
    m[0][0] = v.x;
    m[1][1] = v.y;
    m[2][2] = v.z;
    return m;
}

Matrix4x4f Rx(float angle)
{
    Matrix4x4f m;
    m[1][1] = cos(angle);
    m[1][2] = -sin(angle);
    m[2][1] = sin(angle);
    m[2][2] = cos(angle);
    return m;
}

Matrix4x4f Ry(float angle)
{
    Matrix4x4f m;
    m[0][0] = cos(angle);
    m[0][2] = sin(angle);
    m[2][0] = -sin(angle);
    m[2][2] = cos(angle);
    return m;
}

Matrix4x4f Rz(float angle)
{
    Matrix4x4f m;
    m[0][0] = cos(angle);
    m[0][1] = -sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
    return m;
}
