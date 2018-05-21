#ifndef UTILS
#define UTILS

#include "matrix.h"
#include <math.h>
#include <sys/time.h>

# define PI           3.14159265358979323846  /* pi */

typedef struct timeval timestamp;


inline
Vector3f reflect(const Vector3f &I, const Vector3f &N)
{
    //I-2(N.I)N => N.I = cosi
    return (I - 2.0f * I.dot(N) * N).normalize();
}

inline
Vector3f refract(const Vector3f &I, const Vector3f &N, float n1, float n2)
{
    //n  = n1 / n2
    float n = n1 / n2;

    //cosi = N.I
    float cosi = N.dot(I);

    // sin2t = n^2(1-cosi^2)
    float sin2t = n * n * (1.0f - cosi * cosi);

    if (sin2t > 1.0f) return Vector3f(0);

    return n * I + (n * cosi - sqrt(1.0f - sin2t)) * N;
}

inline
void fresnel(const Vector3f &I, const Vector3f &N, const float &ior, float &kr)
{
    float cosi = N.dot(I);
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

inline
float clamp(float x)
{
    return x < 0.0f ? 0.0f : x > 1.0 ? 1.0f : x;
}

inline
double ms_time()
{
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return (double) currentTime.tv_sec * 1000.0 + (double)currentTime.tv_usec/1000.0;
}

inline
float deg2rad(float deg)
{
    return deg * PI / 180.0;
}

inline
float rad2deg(float rad)
{
    return rad * 180.0 / PI;
}
inline
Matrix4x4f T(const Vector3f& v)
{
    Matrix4x4f m;
    m[0][3] = v.x;
    m[1][3] = v.y;
    m[2][3] = v.z;
    return m;
}
inline
Matrix4x4f S(const Vector3f& v)
{
    Matrix4x4f m;
    m[0][0] = v.x;
    m[1][1] = v.y;
    m[2][2] = v.z;
    return m;
}
inline
Matrix4x4f Rx(float angle)
{
    Matrix4x4f m;
    m[1][1] = cos(angle);
    m[1][2] = -sin(angle);
    m[2][1] = sin(angle);
    m[2][2] = cos(angle);
    return m;
}
inline
Matrix4x4f Ry(float angle)
{
    Matrix4x4f m;
    m[0][0] = cos(angle);
    m[0][2] = sin(angle);
    m[2][0] = -sin(angle);
    m[2][2] = cos(angle);
    return m;
}
inline
Matrix4x4f Rz(float angle)
{
    Matrix4x4f m;
    m[0][0] = cos(angle);
    m[0][1] = -sin(angle);
    m[1][0] = sin(angle);
    m[1][1] = cos(angle);
    return m;
}

template<typename T>
T **new_array_2d(size_t width, size_t height)
{
    T** array2d_ptr = new T *[height];
    for(size_t i = 0; i < height; ++i)
        array2d_ptr[i] = new T[width]{0};
    return array2d_ptr;
}

template<typename T>
void del_array_2d(T **array2dptr, size_t height)
{
    for(size_t i = 0; i <height; i++)
        delete [] array2dptr[i];
    delete [] array2dptr;
}

#endif // UTILS
