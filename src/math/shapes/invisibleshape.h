#pragma once

#include "shape.h"

class InvisibleShape: public ShapeIF
{
    // IntersectionIF interface
public:
    bool intersection(const Ray &, IntersectionData &) const override;
    bool intersection(const Ray &, float &) const override;

    // Shape interface
public:
    Vector3 normal(const Vector3 &, size_t) const override;
    Vector2 uv(const Vector3 &, size_t) const override;
protected:
    InvisibleShape() = default;
    InvisibleShape(const InvisibleShape&) = delete;
    InvisibleShape& operator=(const InvisibleShape&) = delete;

    friend class Shapes;
};

