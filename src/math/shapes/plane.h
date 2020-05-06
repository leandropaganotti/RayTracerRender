#pragma once

#include "shape.h"

class Plane: public ShapeWithMaterial
{
public:               
    bool intersection(const Ray& ray, float tmax, IntersectionData& isec) const;
    bool intersection(const Ray& ray, float tmax) const;
    Vector3 normal(const Vector3 &, size_t) const;
    Vector2 uv(const Vector3& phit, size_t) const;
    void fetch(const Ray &ray, IntersectionData &isec) const override;

    Vector3 getO() const;
    void setO(const Vector3 &value);

    Vector3 getN() const;
    void setN(const Vector3 &value);

protected:
    Plane(const Vector3& O={0.0f}, const Vector3& N={0.0f, 1.0f, 0.0f});
    Vector3 O;    // a point O(origin) on the plane
    Vector3 N;    // normal to the plane

    friend class Shapes;
};


