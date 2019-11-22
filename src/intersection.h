#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vector.h"

class IntersectionData;
class Object;
class Ray;

class IntersectionInt{
public:
    virtual ~IntersectionInt(){ }
    virtual bool intersection(const Ray& ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray& ray, float &tnear) const = 0;
};

class IntersectionData
{
public:
    float tnear;
    size_t idx;
    const Object * object;
    Vector3 phit;
    Vector3 normal;
};

#endif // INTERSECTION_H
