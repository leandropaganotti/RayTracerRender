#pragma once

#include <stddef.h>
#include "ray.h"
#include "vector.h"
#include "intersection.h"
#include "aabb.h"
#include "material.h"

class IntersectionIF
{
public:
    virtual bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray &ray, float tmax) const = 0;
    virtual AABB getAABB() const = 0;
    virtual ~IntersectionIF(){};
};

class Shape: public IntersectionIF
{
public:
    virtual Vector3         getNormal(const Vector3 &phit, size_t idx) const = 0;

    virtual void            getIsecData(const Ray &ray, IntersectionData &isec) const
    {
        isec.phit = ray.origin + isec.tnear * ray.direction;
        isec.normal = getNormal(isec.phit, isec.idx);
        isec.material = getMaterial(isec.idx);
        isec.uv = getUV(isec.phit, isec.idx);
    }

    virtual const Material* getMaterial(size_t) const
    {
        return Material::DiffuseWhite.get();
    }
    virtual Vector2         getUV(const Vector3 &, size_t) const
    {
        return Vector2(0);
    }

    virtual ~Shape(){};
};




