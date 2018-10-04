#ifndef AABB_H
#define AABB_H

#include "box.h"

class AABB
{
public:
    AABB() = default;
    AABB(Vector3 min, Vector3 max);

    Vector3 getCenter();
    const Vector3& getMin() const;
    const Vector3& getMax() const;

    void setMin(const Vector3 &value);
    void setMax(const Vector3 &value);

    bool intersection(const Ray &ray, float &tnear) const;
    void create(const std::vector<Vector3>& vertices);

private:
    Vector3 min;
    Vector3 max;
};

inline
bool AABB::intersection(const Ray &ray, float &tnear) const
{
    Vector3 invdir = 1.0f / ray.direction ;

    float t1 = (min.x - ray.origin.x)*invdir.x;
    float t2 = (max.x - ray.origin.x)*invdir.x;
    float t3 = (min.y - ray.origin.y)*invdir.y;
    float t4 = (max.y - ray.origin.y)*invdir.y;
    float t5 = (min.z - ray.origin.z)*invdir.z;
    float t6 = (max.z - ray.origin.z)*invdir.z;

    float tminx, tminy, tminz, tmin;

    if(t1 < t2) { tminx = t1; } else { tminx = t2; }

    if(t3 < t4) { tminy = t3; } else { tminy = t4; }

    if(t5 < t6) { tminz = t5; } else { tminz = t6; }

    if ( tminx > tminy ) { tmin = tminx; } else { tmin = tminy; }

    if ( tminz > tmin ) { tmin = tminz;}

    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    // if tmax < 0, it is behind the ray
    if (tmax < 0) { return false; }

    // if tmin > tmax, ray doesn't intersect
    if (tmin > tmax) { return false; }

    // if tmin < 0 then the ray origin is inside of the box and tmin is behind the start of the ray so tmax is the first intersection
    if(tmin < 0) { tnear = tmax; } else { tnear = tmin; }
    return true;
}



#endif // AABB_H
