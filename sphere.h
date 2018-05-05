#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere: public Object
{
    Vector3f center;                        // position of the sphere
    float radius, radius2;                  // sphere radius and radius^2
public:    
    Sphere( const Vector3f &center={0.0f}, const float &radius=1.0f, const Vector3f &color={1.0f});

    bool  intersection(const Ray &ray, IntersectionData &isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;
};

#endif  // SPHERE_H
