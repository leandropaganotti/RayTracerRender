#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray(){}
    Ray(const Vector3f& origin, const Vector3f& direction): origin(origin), direction(direction){}

    friend std::ostream& operator<< (std::ostream& os, const Ray& r)
    {
        return os << r.origin << " " << r.direction ;
    }

    Vector3f origin;
    Vector3f direction;
};

#endif // RAY_H
