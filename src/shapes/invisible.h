#pragma once

#include "shape.h"
#include <memory>

class InvisibleShape: public Shape
{
    InvisibleShape();
    static std::shared_ptr<InvisibleShape> instance;
public:
    static std::shared_ptr<InvisibleShape> GetInstance();
    bool intersection(const Ray &, IntersectionData &) const override;
    bool intersection(const Ray &) const override;
    void getIsecData(IntersectionData &isec) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData &) const override;
    AABB getAABB() const override;
};
