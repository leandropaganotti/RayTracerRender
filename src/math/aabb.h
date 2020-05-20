#pragma once
#include <vector>
#include "vector.h"
#include "ray.h"

class AABB
{
public:    
    AABB(const Vector3 min={0.0f}, const Vector3 max={0.0f});
    void extend(const std::vector<Vector3>& vertices);
    void extend(const Vector3 &v);
    void extend(const AABB &aabb);
    bool intersection(const Ray &ray, float tmax) const;

    Vector3 getCenter();

    Vector3 getMin() const;
    void setMin(const Vector3 &value);

    Vector3 getMax() const;
    void setMax(const Vector3 &value);

protected:
    Vector3 min;
    Vector3 max;
};
