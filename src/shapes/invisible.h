#pragma once

#include "primitive.h"
#include <memory>

class InvisibleShape: public Primitive
{
public:
    InvisibleShape() = default;
    ~InvisibleShape() = default;

    bool intersection(const Ray &, IntersectionData &) const override;
    bool intersection(const Ray &) const override;
    void getNormalAndUV(IntersectionData &isec) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData &) const override;
    AABB getAABB() const override;
};
