#pragma once

#include "shape.h"
#include <memory>

class InvisibleShape: public ShapeNormalUV
{
    InvisibleShape();
    static std::shared_ptr<InvisibleShape> instance;
public:
    static std::shared_ptr<InvisibleShape> GetInstance();
    bool intersection(const Ray &, float tmax, IntersectionData &) const override;
    bool intersection(const Ray &, float ) const override;
    void fetchData(const Ray &ray, IntersectionData &isec) const override;
    Vector3 getNormal(const Vector3 &phit, size_t idx) const override;
    Vector2 getUV(const Vector3 &phit, size_t idx) const override;
    AABB getAABB() const override;
};

