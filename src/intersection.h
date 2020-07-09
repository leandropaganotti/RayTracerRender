#pragma once

#include "vector.h"
#include "material.h"

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

    inline Vector3 Ka() const { return albedo * material->Ka;}
    inline Vector3 Kd() const { return albedo;}
    inline Vector3 Ks() const { return material->Ks;}
    inline float   Ns() const { return material->Ns;}
};

class IntersectionIF
{
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray &ray) const = 0;
    virtual AABB getAABB() const = 0;
    virtual ~IntersectionIF(){};
};
