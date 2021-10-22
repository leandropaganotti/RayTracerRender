#pragma once

#include "shape.h"
#include <memory>

class UnitYCylinder: public Shape
{
public:
    UnitYCylinder() = default;
    ~UnitYCylinder() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    void getNormalAndUV(IntersectionData &isec) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData &) const override;
    AABB getAABB() const override;

protected:
    static constexpr float r = 0.5;
    static constexpr float r2 = r*r;
    static constexpr float ymin = -0.5;
    static constexpr float ymax = 0.5;
};
