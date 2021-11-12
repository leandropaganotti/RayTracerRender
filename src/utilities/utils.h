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
float clamp(float x, float min=0.0f, float max=1.0f)
{
    return x < min ? min : x > max ? max : x;
}

inline
void clamp(Vector3 &v)
{
     v.x = clamp(v.x);
     v.y = clamp(v.y);
     v.z = clamp(v.z);
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

// unit sphere
inline
Vector3 uniformSampleSphere(const Vector2 &u, float &_1_pdf) {
    float z = 1 - 2 * u[0];
    float r = std::sqrt(std::max(0.0f, 1.0f - z * z));
    float phi = 2 * M_PI * u[1];
    _1_pdf = 4.0f*M_PI;
    return Vector3(r * std::cos(phi), r * std::sin(phi), z);
}

// unit hemisphere
inline
Vector3 uniformSampleHemisphere(const Vector2 &u, float &_1_pdf) {
    float r = std::sqrt(std::max(0.0f, 1.0f - u[0] * u[0]));
    float phi = 2.0f * M_PI * u[1];
    _1_pdf = 1.0f / (2.0f * M_PI);
    return Vector3(r * std::cos(phi), u[0], r * std::sin(phi));
}

inline
Vector3 uniformSampleHemisphere(const Vector3& normal, float &_1_pdf)
{
    Vector3 u,v, w=normal, n(1,0,0),m(0,1,0);
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;
    Vector3 sample = uniformSampleHemisphere(Vector2(dis(gen), dis(gen)), _1_pdf);
    return sample.x*u + sample.y*w + sample.z*v;
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

// sample direction
inline
Vector3 sampleSolidAngleSphere(const Vector3 &center, float radius, const Vector3 &from, float &_1_pdf)
{
    float r1 = dis(gen);
    float r2 = dis(gen);

    Vector3 u,v, w=(center - from).normalize(), n(1,0,0),m(0,1,0);
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;

    float dist2 = (center - from).length2(); // distance from point hit to center of light power 2
    float cos_a_max = std::sqrt(std::max(0.0f, 1.0f - (radius*radius)/dist2));
    float cos_a = (1.0f - r1) + r1 * cos_a_max;
    float sin_a = std::sqrt(std::max(0.0f, 1.0f - cos_a*cos_a));
    float phi = 2.0f * M_PI * r2;
    _1_pdf = (2.0f*M_PI*(1.0f-cos_a_max));//1/(2*M_PI*(1-cos_a_max));
    //_1_pdf = 1.0f/(2.0f*M_PI*(1.0f-cos_a_max));
    return u*(cos(phi)*sin_a) + v*(sin(phi)*sin_a) + w*(cos_a);
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
