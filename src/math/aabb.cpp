#include "aabb.h"
#include <float.h>
#include <algorithm>

Vector3 AABB::getCenter()
{
    return (min + max) * 0.5f;
}

Vector3 AABB::getMin() const
{
    return min;
}

void AABB::setMin(const Vector3 &value)
{
    min = value;
}

Vector3 AABB::getMax() const
{
    return max;
}

void AABB::setMax(const Vector3 &value)
{
    max = value;
}

void AABB::extend(const std::vector<Vector3> &vertices)
{
    for(const auto &v: vertices)
    {
        extend(v);
    }
}

void AABB::extend(const Vector3 &vertex)
{
    if (vertex.x < min.x)
        min.x = vertex.x;
    if (vertex.y < min.y)
        min.y = vertex.y;
    if (vertex.z < min.z)
        min.z = vertex.z;

    if (vertex.x > max.x)
        max.x = vertex.x;
    if (vertex.y > max.y)
        max.y = vertex.y;
    if (vertex.z > max.z)
        max.z = vertex.z;
}

void AABB::extend(const AABB &aabb)
{
    extend(aabb.getMin());
    extend(aabb.getMax());
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

AABB::AABB(const Vector3 min, const Vector3 max): min(min), max(max)
{

}
