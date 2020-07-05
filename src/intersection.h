#pragma once

#include "vector.h"

class Shape;
class Material;
class Ray;
class AABB;
class Object;

struct IntersectionData
{
    IntersectionData()
    {
        tnear = INFINITY;
        shape = nullptr;
        object = nullptr;
        material = nullptr;
    }
    float tnear;
    size_t idx;
    const Shape * shape;
    const Object * object;
    Vector3 phit;
    Vector3 normal;
    const Material * material;
    Vector2 uv;
    Vector3 albedo;
};

class IntersectionIF
{
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray &ray) const = 0;
    virtual AABB getAABB() const = 0;
    virtual ~IntersectionIF(){};
};
