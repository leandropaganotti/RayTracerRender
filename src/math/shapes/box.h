#pragma once

#include "shape.h"

/*
 * Axis-Aligned Box
*/
class AABox: public Shape
{
    // IntersectionIF interface
public:
    bool intersection(const Ray &ray, IntersectionData &isec) const;
    bool intersection(const Ray &ray, float &tnear) const;

    // Shape interface
public:
    Vector3 normal(const Vector3 &phit, size_t idx) const;
    std::pair<float, float> uv(const Vector3 &phit, size_t idx) const;

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

class Box: public LocalInstance
{
protected:
    Box();
    friend class Shapes;
};
