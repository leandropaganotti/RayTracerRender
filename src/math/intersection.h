#pragma once

#include "vector.h"

class Object;
class Ray;

struct IntersectionData
{
    float tnear;
    size_t idx;
    const Object * object;
    Vector3 phit;
    Vector3 normal;
};

class IntersectionIF
{
public:
	IntersectionIF() = default;
    virtual ~IntersectionIF() = default;
    virtual bool intersection(const Ray& ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray& ray, float &tnear) const = 0;    
};
