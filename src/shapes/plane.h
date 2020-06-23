#pragma once

#include "shape.h"
#include <memory>

class Plane: public Shape
{
public:
    Plane(const Vector3& origin={0.0f}, const Vector3& normal={0.0f, 1.0f, 0.0f});
    virtual ~Plane();

    bool intersection(const Ray& ray, IntersectionData& isec) const override;
    bool intersection(const Ray& ray) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData& isec) const override;
    AABB getAABB() const override;

    Vector3 getOrigin() const;
    void setOrigin(const Vector3 &value);

    Vector3 getNormal() const;
    void setNormal(const Vector3 &value);

protected:
    Vector3 origin;     // a point O(origin) on the plane
    Vector3 w;          // normal to the plane
    Vector3 u;          // orthonormal bases
    Vector3 v;
};

class GPlane: public Plane
{
public:
    GPlane(const Vector3& origin={0.0f}, const Vector3& normal={0.0f, 1.0f, 0.0f});
    virtual ~GPlane();

    void getIsecData(const Ray &ray, IntersectionData &isec) const override;

    void setMaterial(const std::shared_ptr<Material> &value);

    const Material* getMaterial(size_t) const override;

protected:
    std::shared_ptr<Material> material;
};
