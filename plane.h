#ifndef PLANE_H
#define PLANE_H

#include "object.h"

class Plane: public Object
{
    Vector3f P;         // a point P on the plane
    Vector3f N;    // normal to the plane
public:    
    Plane(const Vector3f& P={0.0f}, const Vector3f& N={0.0f, 1.0f, 0.0f}, const Vector3f& color={1.0f});

    bool intersection(const Ray& ray, IntersectionData& isec) const;
    bool intersection(const Ray& ray, float &tnear) const;
    const Vector3f normal(const Vector3f &, size_t) const
    {
        return N;
    }

    static bool intersection(const Vector3f& P, const Vector3f& n, const Ray& ray, float& tnear)
    {
        float t = ((P-ray.origin) ^ n) / (ray.direction ^ n);

        if ( t > 0.0f )
        {
            tnear = t;
            return true;
        }
        return false;
    }
};

#endif // PLANE_H

