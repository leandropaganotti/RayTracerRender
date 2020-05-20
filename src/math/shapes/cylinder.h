#pragma once

#include "shape.h"
#include "instance.h"

class UnitYCylinder: public ShapeNormalUV
{
public:
    UnitYCylinder();

    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;
    Vector3 getNormal(const Vector3 &phit, size_t idx) const override;
    Vector2 getUV(const Vector3 &phit, size_t idx) const override;
    virtual void fetchData(const Ray &ray, IntersectionData &isec) const override;
    AABB getAABB() const override;
};

class GCylinder: public Instance
{
public:
    GCylinder();

    void fetchData(const Ray &ray, IntersectionData &isec) const override;

    std::shared_ptr<Material> getMaterial() const;
    void setMaterial(const std::shared_ptr<Material> &value);

protected:
    std::shared_ptr<Material> material;
};
