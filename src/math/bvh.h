#pragma once

#include "shape.h"
#include <vector>
#include "mesh.h"
#include "aabb.h"

class BVH: public Shape
{
public:
    ~BVH();

    static Shape* Create(std::vector<TriangleMesh> &shapes, size_t l, size_t r, size_t axis);
    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;
private:
    BVH();
    AABB aabb;
    Shape *left;
    Shape *right;

    static size_t qsplit(std::vector<TriangleMesh> &shapes, size_t l, size_t r, float pivot, size_t axis);

    Vector3 normal(const Vector3 &phit, size_t idx) const override;
    Vector2 uv(const Vector3 &phit, size_t idx) const override;
    void fetch(const Ray &ray, IntersectionData &isec) const override;
};
