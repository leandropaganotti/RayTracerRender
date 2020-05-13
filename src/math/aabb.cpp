#include "aabb.h"
#include <float.h>
#include <algorithm>

Vector3 AABB::getCenter()
{
    return (min + max) * 0.5f;
}

void AABB::create(const std::vector<Vector3> &vertices)
{
    min.x=FLT_MAX; max.x=FLT_MIN;
    min.y=FLT_MAX; max.y=FLT_MIN;
    min.z=FLT_MAX; max.z=FLT_MIN;

    for(size_t i= 0 ; i < vertices.size(); ++i)
    {
        if (vertices[i].x < min.x)
            min.x = vertices[i].x;
        if (vertices[i].y < min.y)
            min.y = vertices[i].y;
        if (vertices[i].z < min.z)
            min.z = vertices[i].z;

        if (vertices[i].x > max.x)
            max.x = vertices[i].x;
        if (vertices[i].y > max.y)
            max.y = vertices[i].y;
        if (vertices[i].z > max.z)
            max.z = vertices[i].z;
    }
}

void AABB::extend(const Vector3 &v)
{
    if (v.x < min.x)
        min.x = v.x;
    if (v.y < min.y)
        min.y = v.y;
    if (v.z < min.z)
        min.z = v.z;

    if (v.x > max.x)
        max.x = v.x;
    if (v.y > max.y)
        max.y = v.y;
    if (v.z > max.z)
        max.z = v.z;
}

bool AABB::intersection(const Ray &ray, float tmax) const
{
    Vector3 invdir = 1.0f / ray.direction;

    float t1 = (min.x - ray.origin.x)*invdir.x;
    float t2 = (max.x - ray.origin.x)*invdir.x;
    float t3 = (min.y - ray.origin.y)*invdir.y;
    float t4 = (max.y - ray.origin.y)*invdir.y;
    float t5 = (min.z - ray.origin.z)*invdir.z;
    float t6 = (max.z - ray.origin.z)*invdir.z;

    float tmin_ = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax_ = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax_ < 0 || tmin_ > tmax_) { return false; }

    float tval;
    if(tmin_ < 0) { tval = tmax_; } else { tval = tmin_; }

    if (tval > tmax) return false;

    return true;
}

AABB::AABB(const Vector3 &min, const Vector3 &max): min(min), max(max)
{

}
