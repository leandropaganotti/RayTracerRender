#pragma once

class AABB;
class Ray;
class IntersectionData;

class Intersection
{
public:
    Intersection() = default;
    virtual ~Intersection() = default;

    virtual bool intersection(const Ray &ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray &ray) const = 0;
    virtual AABB getAABB() const = 0;
};
