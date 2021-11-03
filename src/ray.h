#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray(){}
    Ray(const Vector3& o, const Vector3& d, float tmax=INFINITY): origin(o), direction(d), tmax(tmax)
    {
        invdir[0] = 1.0f / direction[0];
        invdir[1] = 1.0f / direction[1];
        invdir[2] = 1.0f / direction[2];
        posneg[0] = direction[0] > 0 ? 0 : 1;
        posneg[1] = direction[1] > 0 ? 0 : 1;
        posneg[2] = direction[2] > 0 ? 0 : 1;
    }

    Vector3 origin;
    Vector3 direction;
    mutable float tmax;
    Vector3 invdir;
    int posneg[3];

    friend std::ostream& operator<< (std::ostream& os, const Ray& r)
    {
        return os << "o:" << r.origin << ", d:" << r.direction << ", tmax=" << r.tmax  << ", invdir:" << r.invdir << ", posneg:" << r.posneg[0] << r.posneg[1] << r.posneg[2];
    }
};

#endif // RAY_H
