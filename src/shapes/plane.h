#pragma once

#include "shape.h"
#include <memory>

class Plane: public Shape
{
public:
    Plane(const Vector3& origin=vector::ZERO, const Vector3& normal=vector::BACK);
    ~Plane() = default;

    bool intersection(const Ray& ray, IntersectionData& isec) const override;
    bool intersection(const Ray& ray) const override;
    void getNormalAndUV(IntersectionData &isec) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData& isec) const override;
    AABB getAABB() const override;

    Vector3 getOrigin() const;
    void    setOrigin(const Vector3 &value);

    Vector3 getNormal() const;
    void    setNormal(const Vector3 &value);

protected:
    Vector3 origin;     // a point O(origin) on the plane

    // orthonormal bases
    Vector3 u;
    Vector3 v;
    Vector3 w;
};
