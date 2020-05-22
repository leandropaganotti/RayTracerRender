#pragma once

#include "shape.h"
#include <memory>

class Plane: public Shape
{
public:
    Plane(const Vector3& O={0.0f}, const Vector3& N={0.0f, 1.0f, 0.0f});
    virtual ~Plane();

    bool intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool intersection(const Ray& ray, float tmax) const override;
    Vector3 getNormal(const Vector3 &, size_t) const override;
    Vector2 getUV(const Vector3& phit, size_t) const override;
    AABB getAABB() const override;

    Vector3 getO() const;
    void setO(const Vector3 &value);

    Vector3 getN() const;
    void setN(const Vector3 &value);

protected:
    Vector3 O;    // a point O(origin) on the plane
    Vector3 N;    // normal to the plane
};

class GPlane: public Plane
{
public:
    GPlane(const Vector3& O={0.0f}, const Vector3& N={0.0f, 1.0f, 0.0f});
    virtual ~GPlane();

    void getIsecData(const Ray &ray, IntersectionData &isec) const override;

    void setMaterial(const std::shared_ptr<Material> &value);

    const Material* getMaterial(size_t) const override;

protected:
    std::shared_ptr<Material> material;
};