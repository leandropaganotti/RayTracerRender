#include "aabb.h"
#include <float.h>
#include <algorithm>

Vector3 AABB::getCenter()
{
    return (data[0] + data[1]) * 0.5f;
}

Vector3 AABB::getMin() const
{
    return data[0];
}

void AABB::setMin(const Vector3 &value)
{
    data[0] = value;
}

Vector3 AABB::getMax() const
{
    return data[1];
}

void AABB::setMax(const Vector3 &value)
{
    data[1] = value;
}

Vector3 AABB::getExtent()
{
    return data[1]-data[0];
}

int AABB::getMaxExtent()
{
    Vector3 e = getExtent();
    if (e.x > e.y && e.x > e.z)
        return 0;
    else if (e.y > e.z)
        return 1;
    else
        return 2;
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
    if (vertex.x < data[0].x)
        data[0].x = vertex.x;
    if (vertex.y < data[0].y)
        data[0].y = vertex.y;
    if (vertex.z < data[0].z)
        data[0].z = vertex.z;

    if (vertex.x > data[1].x)
        data[1].x = vertex.x;
    if (vertex.y > data[1].y)
        data[1].y = vertex.y;
    if (vertex.z > data[1].z)
        data[1].z = vertex.z;
}

void AABB::extend(const AABB &aabb)
{
    extend(aabb.getMin());
    extend(aabb.getMax());
}

bool AABB::intersection(const Ray &ray, float tmax) const
{
    float imin = 0;
    float imax = tmax;

    int posneg = ray.posneg[0];
    float t0 = (data[posneg].x - ray.origin.x) * ray.invdir.x;
    float t1 = (data[1-posneg].x - ray.origin.x) * ray.invdir.x;
    if(t0 > imin) imin = t0;
    if(t1 < imax) imax = t1;
    if(imin > imax) return false;

    posneg = ray.posneg[1];
    t0 = (data[posneg].y - ray.origin.y) * ray.invdir.y;
    t1 = (data[1-posneg].y - ray.origin.y) * ray.invdir.y;
    if(t0 > imin) imin = t0;
    if(t1 < imax) imax = t1;
    if(imin > imax) return false;

    posneg = ray.posneg[2];
    t0 = (data[posneg].z - ray.origin.z) * ray.invdir.z;
    t1 = (data[1-posneg].z - ray.origin.z) * ray.invdir.z;
    if(t0 > imin) imin = t0;
    if(t1 < imax) imax = t1;
    return imin <= imax;
}

AABB::AABB()
{
    data[0] = data[1] =  Vector3(0);
}

AABB::AABB(const Vector3 &min, const Vector3 &max)
{
    data[0] = min;
    data[1] = max;
}

void AABB::reset(const std::vector<Vector3> &vertices)
{
    data[0] = data[1] =  Vector3(0);
    extend(vertices);
}
