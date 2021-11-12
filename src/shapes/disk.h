#pragma once

#include "plane.h"

class Disk: public Plane
{
public:
    Disk(const Vector3 &origin, const Vector3 &normal, float radius);
private:
    float radius;

    // Shape interface
public:
    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    AABB getAABB() const override;
    void getSample(const Vector3 &from, Vector3 &dir, float &t, float &_1_pdf) const override;
};

