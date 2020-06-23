#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray(){tmax=INFINITY;};
    Ray(const Vector3& o, const Vector3& d): origin(o), direction(d), tmax(INFINITY){}
    Ray(const Vector3& o, const Vector3& d, float tmax): origin(o), direction(d), tmax(tmax){}

    Vector3 origin;
    Vector3 direction;
    mutable float tmax;

    Vector3 invdir;
    int posneg[3];

    friend std::ostream& operator<< (std::ostream& os, const Ray& r)
    {
        return os << r.origin << " " << r.direction ;
    }
};

#endif // RAY_H
