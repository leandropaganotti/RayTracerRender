#pragma once
#include <vector>
#include "vector.h"
#include "ray.h"

class AABB
{
public:    
    AABB(const Vector3 &min={0.0f}, const Vector3 &max={0.0f});

    void create(const std::vector<Vector3>& vertices);
    void extend(const Vector3 &v);
    bool intersection(const Ray &ray) const;
    Vector3 getCenter();
protected:
    Vector3 min;
    Vector3 max;
};
