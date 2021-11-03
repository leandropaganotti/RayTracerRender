#pragma once

#include <ctime>
#include "matrix.h"
#include <sstream>
#include <vector>
#include <random>
#include <cmath>
#include <iostream>

namespace {
std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<float> dis(0, 1);
}

#define UNUSED(expr) (void)(expr)

struct RNG
{
    RNG()
    {
        srand(time(0));
    }
    float operator()()
    {
        return float(rand()) / float(RAND_MAX);
    }
};

typedef struct timeval timestamp;


inline
Vector3 reflect(const Vector3 &I, const Vector3 &N)
{
    //I-2(N.I)N => N.I = cosi
    return (I + 2.0f * -I.dot(N) * N).normalize();
}

inline
Vector3 refract(const Vector3 &I, const Vector3 &N, float n1, float n2)
{
    //n  = n1 / n2
    float n = n1 / n2;

    //cosi = N.I
    float cosi = -I.dot(N);

    // sin2t = n^2(1-cosi^2)
    float sin2t = n * n * std::max(0.0f, 1.0f - cosi * cosi);

    if (sin2t >= 1.0f) return Vector3(0);// TIR

    return n * I + (n * cosi - sqrt(1.0f - sin2t)) * N;
}

inline
float fresnel(const Vector3 &I, const Vector3 &N, float etai, float etat)
{
    float Re;
    float cosThetaI = - I.dot(N);
    // Compute sini using Snell's law
    float sinThetaT = etai / etat * std::sqrt(std::max(0.f, 1 - cosThetaI * cosThetaI));
    // Total internal reflection
    if (sinThetaT >= 1) {
        Re = 1.0;
    } else {
        float cosThetaT = sqrtf(std::max(0.f, 1 - sinThetaT * sinThetaT));
        float Rs = ((etai * cosThetaI) - (etat * cosThetaT)) / ((etai * cosThetaI) + (etat * cosThetaT));
        float Rp = ((etat * cosThetaI) - (etai * cosThetaT)) / ((etat * cosThetaI) + (etai * cosThetaT));
        Re = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
    return Re;

}

inline
float schlick(const Vector3 &I, const Vector3 &N, const float R0)
{
    float c = 1.0f + I.dot(N); // 1.0 - -cosThetaI
    return R0 + (1.0f - R0) * (c*c*c*c*c);
}

inline
float schlick(const Vector3 &I, const Vector3 &N, const float n1, const float n2)
{
    float cosThetaX = -I.dot(N); // cosThetaI
    if (n1 > n2){
        const float n = n1 / n2;
        const float sin2ThetaT = n * n * (1.0 - (cosThetaX * cosThetaX));
        if (sin2ThetaT > 1.0) return 1.0; // TIR
        cosThetaX = std::sqrt(1.0 - sin2ThetaT * sin2ThetaT);
    }

    float R0 = (n1-n2) / (n1 + n2);
    R0 *= R0;
    float c = 1.0 - cosThetaX;
    return R0 + (1.0f - R0) * (c*c*c*c*c);;
}

inline
float modulo(const float x)
{
    return x - std::floor(x);
}

inline
float clamp(float x)
{
    return x < 0.0f ? 0.0f : x > 1.0 ? 1.0f : x;
}

inline
float deg2rad(float deg)
{
    return deg * M_PI / 180.0;
}

inline
float rad2deg(float rad)
{
    return rad * 180.0 / M_PI;
}


template<typename T>
T **new_array_2d(size_t width, size_t height)
{
    if (!width || !height)
        return NULL;
    T** array2d_ptr = new T *[height];
    for(size_t i = 0; i < height; ++i)
        array2d_ptr[i] = new T[width];
    return array2d_ptr;
}

template<typename T>
void del_array_2d(T **array2dptr, size_t height)
{
    for(size_t i = 0; i <height; i++)
        delete [] array2dptr[i];
    delete [] array2dptr;
    array2dptr = NULL;
}

inline
bool solveQuadratic(const float a, const float b, const float c, float &x0, float &x1)
{
    float delta = b * b - 4 * a * c;

    if (delta < 0)
        return false;
    else if (delta == 0)
    {
        x0 = x1 = - 0.5 * b / a;
    }
    else
    {
        float q = (b > 0) ? -0.5 * (b + sqrt(delta)) : -0.5 * (b - sqrt(delta));
        x0 = q / a;
        x1 = c / q;
    }
    return true;
}

inline
Vector3 uniformSampleHemisphere(const float &r1, const float &r2)
{
    // cos(theta) = r1 = y
    // cos^2(theta) + sin^2(theta) = 1^2 -> sin(theta) = srtf(1 - cos^2(theta))
    float sinTheta = sqrtf(1 - r1 * r1);
    float phi = 2 * M_PI * r2;    
    return Vector3(sinTheta * cosf(phi), sinTheta * sinf(phi), r1);
}

inline
Vector3 randomUnitVectorInHemisphereOf(const Vector3& normalAtpoint)
{
    Vector3 u,v, w=normalAtpoint, n(1,0,0),m(0,1,0);
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;

    float r1 = dis(gen); // this is cosi
    float r2 = dis(gen);

    Vector3 sample = uniformSampleHemisphere(r1, r2);
    return sample.x*u + sample.y*v + sample.z*w;
}

inline
void createCoordinateSystem(const Vector3 &N, Vector3 &Nt, Vector3 &Nb)
{
    if (std::fabs(N.x) > std::fabs(N.y))
        Nt = Vector3(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
    else
        Nt = Vector3(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
    Nb = N.cross(Nt);
}

/*inline
std::string timestamp2string(std::time_t timestamp, const char* format="%FT%T")
{
    struct tm t;
    if ( gmtime_r( &timestamp, &t ) == nullptr )
    {
        timestamp = 0;
        gmtime_r( &timestamp, &t );
    }
    char buffer[64] = { };
    strftime( buffer, 64, format, &t );
    return buffer;
}*/
