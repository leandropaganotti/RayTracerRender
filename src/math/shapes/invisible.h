#pragma once

#include "shape.h"
#include <memory>

class InvisibleShape: public Shape
{
    InvisibleShape();
    static std::shared_ptr<Shape> instance;
public:
    static std::shared_ptr<Shape> GetInstance();

    bool intersection(const Ray &, float tmax, IntersectionData &) const override;
    bool intersection(const Ray &, float ) const override;
    Vector3 normal(const Vector3 &, size_t) const override;
    Vector2 uv(const Vector3 &, size_t) const override;
    void fetch(const Ray &ray, IntersectionData &isec) const override;
    AABB getAABB() const override;
};

