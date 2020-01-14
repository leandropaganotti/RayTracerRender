#include "plane.h"

Plane::Plane(const Vector3 &O, const Vector3 &N):
    O(O), N(N)
{

}

bool Plane::intersection(const Ray &ray, IntersectionData& isec) const
{
    if( intersection(ray, isec.tnear) )
    {        
        return true;
    }
    return false;
}

inline
bool Plane::intersection(const Ray &ray, float& tnear) const
{
    float t = ((O-ray.origin) ^ N) / (ray.direction ^ N);

    if ( t > 0.0f )
    {
        tnear = t;
        return true;
    }
    return false;
}

Vector3 Plane::normal(const Vector3 &, size_t) const
{
    return N;
}

Vector2 Plane::uv(const Vector3 &phit, size_t) const
{

    Vector3 v = phit - O;

    if(N.x == 1.0f || N.x == -1.0f)
    {
        return {v.y, v.z};
    }
    else if(N.y == 1.0f || N.y == -1.0f)
    {
        return {v.x, v.z};
    }
    else
    {
        return {v.x, v.y};
    }
}
