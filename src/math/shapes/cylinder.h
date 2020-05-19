#pragma once

#include "shape.h"
#include "instance.h"

class UnitYCylinder: public Shape
{
public:
    UnitYCylinder();

    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;
    Vector3 normal(const Vector3 &phit, size_t idx) const override;
    Vector2 uv(const Vector3 &phit, size_t idx) const override;
    virtual void fetch(const Ray &ray, IntersectionData &isec) const override;
    AABB getAABB() const override;
};

class GCylinder: public Instance
{
public:
    GCylinder();

    void fetch(const Ray &ray, IntersectionData &isec) const override;

    std::shared_ptr<Material> getMaterial() const;
    void setMaterial(const std::shared_ptr<Material> &value);

protected:
    std::shared_ptr<Material> material;
};
