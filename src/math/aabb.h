#pragma once

#include <algorithm>
#include "box.h"
#include <vector>

class AABB: public AABox
{
public:    
    AABB(const Vector3 &min={0.0f}, const Vector3 &max={0.0f});

    Vector3 getCenter();
    void create(const std::vector<Vector3>& vertices);
};
