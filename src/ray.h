#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray
{
public:
    Ray() = default;
    Ray(const Vector3& origin, const Vector3& direction): origin(origin), direction(direction){}

    friend std::ostream& operator<< (std::ostream& os, const Ray& r)
    {
        return os << r.origin << " " << r.direction ;
    }

    Vector3 origin;
    Vector3 direction;

    struct ISec {
        Vector3 wPhit;
        Vector3 lPhit;
        float wTnear;
        float lTnear;
        size_t idx;
    };

    ISec isec{{0}, {0}, 0, 0, 0};
};

#endif // RAY_H
