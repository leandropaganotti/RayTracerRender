#pragma once

#include "shape.h"
#include <memory>
/*
 * Axis-Aligned Box
*/
class AABox: public Shape
{
public:
    AABox(const Vector3 &min={-0.5f, -0.5f, -0.5f}, const Vector3 &max={0.5f,0.5f,0.5f});
    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    void getIsecData(IntersectionData &isec) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData &isec) const override;
    AABB getAABB() const override;

public:
    Vector3 getMin() const;
    void    setMin(const Vector3 &value);

    Vector3 getMax() const;
    void    setMax(const Vector3 &value);

protected:
    Vector3 data[2]; // 0 -> min,  1 -> max
};
