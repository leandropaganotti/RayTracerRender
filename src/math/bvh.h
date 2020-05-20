#pragma once

#include "shape.h"
#include <vector>
#include "mesh.h"
#include "aabb.h"

class BVH: public Shape
{
public:
    ~BVH();
    static std::shared_ptr<Shape> Create(const std::vector<std::shared_ptr<ShapeNormalUV>> &shapes);
    static std::shared_ptr<Shape> Create(const std::vector<std::shared_ptr<Shape>> &shapes);
    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;

    AABB getAABB() const override;

private:
    BVH();
    static std::shared_ptr<Shape> Create(std::vector<std::shared_ptr<Shape> > &shapes, size_t l, size_t r, size_t axis);
    AABB aabb;
    std::shared_ptr<Shape> left;
    std::shared_ptr<Shape> right;

    static size_t qsplit(std::vector<std::shared_ptr<Shape> > &shapes, size_t l, size_t r, float pivot, size_t axis);
};
