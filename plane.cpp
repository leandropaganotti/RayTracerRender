#include "plane.h"

Plane::Plane(const Vector3f &P, const Vector3f &n, const Vector3f& color):
    P(P), normal(n)
{
    c_diffuse = color;
}

bool Plane::intersection(const Ray &ray, IntersectionData& isec) const
{
    if( intersection(P, normal, ray, isec.tnear) )
    {
        isec.phit   = ray.origin + isec.tnear * ray.direction;
        isec.normal = normal;
        isec.object = this;
        return true;
    }
    return false;
}

bool Plane::intersection(const Ray &ray, float& tnear) const
{
    return intersection(P, normal, ray, tnear);
}

// static functions down here
bool Plane::intersection(const Vector3f &P, const Vector3f &n, const Ray &ray, float& tnear)
{
    float t = ((P-ray.origin) ^ n) / (ray.direction ^ n);

    if ( t > 0.0f )
    {
        tnear = t;
        return true;
    }
    return false;
}


