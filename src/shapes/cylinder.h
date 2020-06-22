#pragma once

#include "shape.h"
#include "instance.h"

class UnitYCylinder: public Shape
{
public:
    UnitYCylinder();

    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;
    void getNormal(IntersectionData& isec) const override;
    AABB getAABB() const override;

protected:
    float r;
    float r2;
    float ymin;
    float ymax;
};

class GCylinder: public Instance
{
public:
    GCylinder();

    void getIsecData(const Ray &ray, IntersectionData &isec) const override;

    void setMaterial(const std::shared_ptr<Material> &value);

    const Material* getMaterial(size_t idx) const override;

protected:
    std::shared_ptr<Material> material;
};
