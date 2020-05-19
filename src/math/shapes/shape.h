#pragma once

#include <stddef.h>
#include "ray.h"
#include "vector.h"
#include "intersection.h"
#include "aabb.h"

class Shape
{
public:
    virtual bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray &ray, float tmax) const = 0;

    virtual Vector3 normal(const Vector3 &phit, size_t idx) const = 0;
    virtual Vector2 uv(const Vector3 &phit, size_t idx) const = 0;

    virtual void fetch(const Ray &ray, IntersectionData &isec) const = 0;

    virtual AABB getAABB() const = 0;

    virtual ~Shape() {}
};
