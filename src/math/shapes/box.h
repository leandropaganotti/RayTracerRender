#pragma once

#include "shape.h"

/*
 * Axis-Aligned Box
*/
class AABox: public ShapeWithMaterial
{
    // IntersectionIF interface
public:
    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const;
    bool intersection(const Ray &ray, float tmax) const;
    Vector3 normal(const Vector3 &phit, size_t idx) const;
    Vector2 uv(const Vector3 &phit, size_t idx) const;
    void fetch(const Ray &ray, IntersectionData &isec) const override;


public:
    Vector3 getMin() const;
    void setMin(const Vector3 &value);

    Vector3 getMax() const;
    void setMax(const Vector3 &value);

protected:
    AABox(const Vector3 &min={-0.5f, -0.5f, 0.5f}, const Vector3 &max={0.5f,0.5f,-0.5f});
    Vector3 min;
    Vector3 max;

    friend class Shapes;

};
