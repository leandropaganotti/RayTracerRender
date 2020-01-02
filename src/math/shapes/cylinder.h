#pragma once

#include "shape.h"

class UnitCylinder: public Shape
{    
    // IntersectionIF interface
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const override;
    virtual bool intersection(const Ray &ray, float &tnear) const override;

    // Shape interface
public:
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const override;
    virtual std::pair<float, float> uv(const Vector3 &phit, size_t idx) const override;

protected:
    UnitCylinder();    

    friend class Cylinder;
};

class Cylinder: public LocalInstance
{
protected:
    Cylinder();

    friend class Shapes;
};
