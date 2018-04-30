#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray();

    Ray(const Vector3f& origin, const Vector3f& direction);

    Vector3f origin;
    Vector3f direction;

    friend std::ostream& operator<< (std::ostream& os, const Ray& r);
};

#endif // RAY_H
