#pragma once

#include "shape.h"

class InvisibleShape: public Shape
{
public:
    static std::shared_ptr<InvisibleShape> GetInstance();

    // IntersectionIF interface
public:
    bool intersection(const Ray &, IntersectionData &) const override;
    bool intersection(const Ray &, float &) const override;

    // Shape interface
public:
    const Vector3 normal(const Vector3 &, size_t) const override;
    const std::pair<float, float> uv(const Vector3 &, size_t) const override;
private:
    InvisibleShape() = default;
    InvisibleShape(const InvisibleShape&) = delete;
    InvisibleShape& operator=(const InvisibleShape&) = delete;

    static std::shared_ptr<InvisibleShape> instance;
};

