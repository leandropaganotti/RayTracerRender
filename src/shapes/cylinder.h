#pragma once

#include "shape.h"
#include <memory>

class UnitYCylinder: public Shape
{
public:
    UnitYCylinder();
    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    void getNormalAndUV(IntersectionData &isec) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData &) const override;
    AABB getAABB() const override;

protected:
    float r;
    float r2;
    float ymin;
    float ymax;
};
