#pragma once

#include "shape.h"

class Plane: public Shape
{
public:
    Plane(const Vector3& O={0.0f}, const Vector3& N={0.0f, 1.0f, 0.0f});
    virtual ~Plane();

    bool intersection(const Ray& ray, float tmax, IntersectionData& isec) const override;
    bool intersection(const Ray& ray, float tmax) const override;
    Vector3 normal(const Vector3 &, size_t) const override;
    Vector2 uv(const Vector3& phit, size_t) const override;
    virtual void fetch(const Ray &ray, IntersectionData &isec) const override;

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

    void fetch(const Ray &ray, IntersectionData &isec) const override;

    std::shared_ptr<Material> getMaterial() const;
    void setMaterial(const std::shared_ptr<Material> &value);

protected:
    std::shared_ptr<Material> material;
};
