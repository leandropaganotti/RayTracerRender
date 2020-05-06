#pragma once

#include "shape.h"

class InvisibleShape: public ShapeWithMaterial
{
    // IntersectionIF interface
public:
    bool intersection(const Ray &, float tmax, IntersectionData &) const override;
    bool intersection(const Ray &, float ) const override;
    Vector3 normal(const Vector3 &, size_t) const override;
    Vector2 uv(const Vector3 &, size_t) const override;
    void fetch(const Ray &ray, IntersectionData &isec) const override;

protected:
    InvisibleShape() = default;
    InvisibleShape(const InvisibleShape&) = delete;
    InvisibleShape& operator=(const InvisibleShape&) = delete;

    friend class Shapes;
};

