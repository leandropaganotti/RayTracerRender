#pragma once
#include <vector>
#include "vector.h"
#include "ray.h"

class AABB
{
public:    
    AABB();
    AABB(const Vector3 &min, const Vector3 &max);
    void extend(const std::vector<Vector3>& vertices);
    void extend(const Vector3 &v);
    void extend(const AABB &aabb);
    bool intersection(const Ray &ray, float tmax) const;

    Vector3 getCenter();

    Vector3 getMin() const;
    void setMin(const Vector3 &value);

    Vector3 getMax() const;
    void setMax(const Vector3 &value);

    Vector3 getExtent();
    int getMaxExtent();

protected:
    Vector3 data[2]; // 0 -> min,  1 -> max
};
