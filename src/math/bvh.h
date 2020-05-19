#pragma once

#include "shape.h"
#include <vector>
#include "mesh.h"
#include "aabb.h"

class BVH: public Shape
{
public:
    ~BVH();

    static std::shared_ptr<Shape> Create(std::vector<std::shared_ptr<Shape>> &shapes);
    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;

    AABB getAABB() const override;

private:
    BVH();
    static std::shared_ptr<Shape> Create(std::vector<std::shared_ptr<Shape>> &shapes, size_t l, size_t r, size_t axis);
    AABB aabb;
    std::shared_ptr<Shape> left;
    std::shared_ptr<Shape> right;

    static size_t qsplit(std::vector<std::shared_ptr<Shape>> &shapes, size_t l, size_t r, float pivot, size_t axis);

    Vector3 normal(const Vector3 &phit, size_t idx) const override;
    Vector2 uv(const Vector3 &phit, size_t idx) const override;
    void fetch(const Ray &ray, IntersectionData &isec) const override;
};
