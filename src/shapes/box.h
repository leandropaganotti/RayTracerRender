#pragma once

#include "shape.h"
#include "instance.h"

/*
 * Axis-Aligned Box
*/
class AABox: public Shape
{
public:
    AABox(const Vector3 &min={-0.5f, -0.5f, -0.5f}, const Vector3 &max={0.5f,0.5f,0.5f});

    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;
    Vector3 getNormal(const Vector3 &phit, size_t idx) const override;
    Vector2 getUV(const Vector3 &phit, size_t idx) const override;
    AABB getAABB() const override;

public:
    Vector3 getMin() const;
    void setMin(const Vector3 &value);

    Vector3 getMax() const;
    void setMax(const Vector3 &value);

protected:
    Vector3 min;
    Vector3 max;
    Vector3 data[2]; // 0 -> min,  1 -> max
};

class GBox: public Instance
{
public:
    GBox();

    void getIsecData(const Ray &ray, IntersectionData &isec) const override;

    void setMaterial(const std::shared_ptr<Material> &value);

    const Material * getMaterial(size_t) const override;

protected:
    std::shared_ptr<Material> material;
};
