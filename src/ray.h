#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray() = default;
    Ray(const Vector3& origin, const Vector3& direction): origin(origin), direction(direction){}

    Vector3 origin;
    Vector3 direction;

    friend std::ostream& operator<< (std::ostream& os, const Ray& r)
    {
        return os << r.origin << " " << r.direction ;
    }
};

#endif // RAY_H
