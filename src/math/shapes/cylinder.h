#pragma once

#include "shape.h"

class UnitYCylinder: public ShapeWithMaterial
{    
    // IntersectionIF interface
public:
    virtual bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    virtual bool intersection(const Ray &ray, float tmax) const override;
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const override;
    virtual Vector2 uv(const Vector3 &phit, size_t idx) const override;   
    void fetch(const Ray &ray, IntersectionData &isec) const override;

protected:
    UnitYCylinder();

    friend class Shapes;
};
