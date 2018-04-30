#include "ray.h"

Ray::Ray()
{

}

Ray::Ray(const Vector3f &origin, const Vector3f &direction): origin(origin), direction(direction){}

std::ostream& operator<<(std::ostream &os, const Ray &r)
{
    return os << r.origin << " " << r.direction ;
}

