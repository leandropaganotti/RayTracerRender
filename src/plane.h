#ifndef PLANE_H
#define PLANE_H

#include "object.h"
#include "texture.h"

class Plane: public Object
{
public:       
    Plane(const Vector3& O={0.0f}, const Vector3& N={0.0f, 1.0f, 0.0f});

    bool intersection(const Ray& ray, IntersectionData& isec) const;
    bool intersection(const Ray& ray, float &tnear) const;
    const Vector3 normal(const Vector3 &, size_t) const;    

    static bool intersection(const Vector3& O, const Vector3& n, const Ray& ray, float& tnear);

    Vector3 O;    // a point O(origin) on the plane
    Vector3 N;    // normal to the plane

private:
    const std::pair<float, float> uv(const Vector3& phit, size_t) const;

};

inline
const std::pair<float, float> Plane::uv(const Vector3 &phit, size_t) const
{

    Vector3 v = phit - O;

    if(N.x == 1.0f || N.x == -1.0f)
    {
        return std::make_pair(v.y, v.z);
    }
    else if(N.y == 1.0f || N.y == -1.0f)
    {
        return std::make_pair(v.x, v.z);
    }
    else
    {
        return std::make_pair(v.x, v.y);
    }
}

#endif // PLANE_H

