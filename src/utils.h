#ifndef UTILS
#define UTILS

#include <ctime>
#include "matrix.h"
#include <math.h>
#include <sys/time.h>
#include <cmath>
#include <sstream>
#include <vector>

# define PI           3.14159265358979323846  /* pi */

typedef struct timeval timestamp;


inline
Vector3 reflect(const Vector3 &I, const Vector3 &N)
{
    //I-2(N.I)N => N.I = cosi
    return (I - 2.0f * I.dot(N) * N).normalize();
}

inline
Vector3 refract(const Vector3 &I, const Vector3 &N, float n1, float n2)
{
    //n  = n1 / n2
    float n = n1 / n2;

    //cosi = N.I
    float cosi = N.dot(I);

    // sin2t = n^2(1-cosi^2)
    float sin2t = n * n * (1.0f - cosi * cosi);

    if (sin2t > 1.0f) return Vector3(0);

    return n * I + (n * cosi - sqrt(1.0f - sin2t)) * N;
}

inline
void fresnel(const Vector3 &I, const Vector3 &N, const float &ior, float &kr)
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
void createCoordinateSystem(const Vector3 &N, Vector3 &Nt, Vector3 &Nb)
{
    if (std::fabs(N.x) > std::fabs(N.y))
        Nt = Vector3(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
    else
        Nt = Vector3(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
    Nb = N.cross(Nt);
}

inline
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
}

inline
std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    //elems.push_back(item);
    elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
  }
  return std::move(elems);
}
#endif // UTILS
