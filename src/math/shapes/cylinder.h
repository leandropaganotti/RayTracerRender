#pragma once

#include "shape.h"

class UnitYCylinder: public ShapeIF
{    
    // IntersectionIF interface
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const override;
    virtual bool intersection(const Ray &ray, float &tnear) const override;

    // Shape interface
public:
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const override;
    virtual Vector2 uv(const Vector3 &phit, size_t idx) const override;

protected:
    UnitYCylinder();

    friend class Shapes;
};
