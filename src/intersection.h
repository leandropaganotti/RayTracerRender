#pragma once

#include "vector.h"

class Shape;
class Material;
class Ray;
class AABB;

struct IntersectionData
{
    float tnear;
    size_t idx;
    const Shape * shape;

    Vector3 phit;
    Vector3 phit_local;
    Vector3 normal;
    const Material * material;
    Vector2 uv;
    Vector3 color;
};

class IntersectionIF
{
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray &ray) const = 0;
    virtual AABB getAABB() const = 0;
    virtual ~IntersectionIF(){};
};
