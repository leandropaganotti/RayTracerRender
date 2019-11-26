#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vector.h"

class Object;

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
    virtual ~IntersectionIF() = default;
    virtual bool intersection(const Ray& ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray& ray, float &tnear) const = 0;
};

#endif // INTERSECTION_H