#pragma once

#include "shape.h"
#include <vector>
#include "mesh.h"
#include "aabb.h"

class BVH: public IntersectionIF
{
public:
    ~BVH();
    static std::shared_ptr<IntersectionIF> Create(const std::vector<std::shared_ptr<Shape>> &shapes);
    static std::shared_ptr<IntersectionIF> Create(const std::vector<std::shared_ptr<IntersectionIF>> &shapes);
    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;

    AABB getAABB() const override;

private:
    BVH();
    static std::shared_ptr<IntersectionIF> Create(std::vector<std::shared_ptr<IntersectionIF> > &shapes, size_t l, size_t r, size_t axis);
    AABB aabb;
    std::shared_ptr<IntersectionIF> left;
    std::shared_ptr<IntersectionIF> right;

    static size_t qsplit(std::vector<std::shared_ptr<IntersectionIF> > &shapes, size_t l, size_t r, float pivot, size_t axis);
};
