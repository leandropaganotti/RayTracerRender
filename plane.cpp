#include "plane.h"

Plane::Plane(const Vector3f &P, const Vector3f &n, const Vector3f& color):
    P(P), normal(n)
{
    c_diffuse = color;
}

inline
bool Plane::intersection(const Ray &ray, IntersectionData& inter) const
{
    if( intersection(P, normal, ray, inter.tNear) )
    {
        inter.phit   = ray.origin + inter.tNear * ray.direction;
        inter.normal = normal;
        inter.object = this;
        return true;
    }
    return false;
}

inline
bool Plane::intersection(const Ray &ray, float& tNear) const
{
    return intersection(P, normal, ray, tNear);
}

// static functions down here
bool Plane::intersection(const Vector3f &P, const Vector3f &n, const Ray &ray, float& tNear)
{
    float t = ((P-ray.origin) ^ n) / (ray.direction ^ n);

    if ( t > 0.0f )
    {
        tNear = t;
        return true;
    }
    return false;
}


