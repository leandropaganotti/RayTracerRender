#include "plane.h"

Plane::Plane(const Vector3 &O, const Vector3 &n, const Vector3& color):
    Object(color), O(O), N(n)
{

}

bool Plane::intersection(const Ray &ray, IntersectionData& isec) const
{
    if( intersection(O, N, ray, isec.tnear) )
    {
        isec.object = this;
        return true;
    }
    return false;
}

bool Plane::intersection(const Ray &ray, float& tnear) const
{
    return intersection(O, N, ray, tnear);
}

const Vector3 Plane::normal(const Vector3 &, size_t) const
{
    return N;
}

inline
bool Plane::intersection(const Vector3 &O, const Vector3 &n, const Ray &ray, float &tnear)
{
    float t = ((O-ray.origin) ^ n) / (ray.direction ^ n);

    if ( t > 0.0f )
    {
        tnear = t;
        return true;
    }
    return false;
}


