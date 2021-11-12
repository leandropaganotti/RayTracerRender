#pragma once

class Vector3;
class AABB;
class Ray;
class IntersectionData;

class Shape
{
public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual bool intersection(const Ray &ray, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray &ray) const = 0;
    virtual AABB getAABB() const = 0;
    virtual void getSample(const Vector3 &from, Vector3 &dir, float &t, float &_1_pdf) const { _1_pdf = 0.0; };
};
