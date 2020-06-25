#pragma once

#include "intersection.h"
#include "aabb.h"
#include <vector>
#include <memory>

class BVH: public IntersectionIF
{
public:
    ~BVH();
    static std::shared_ptr<IntersectionIF> Create(std::vector<std::shared_ptr<IntersectionIF> > &shapes, size_t l, size_t r);

    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;

    AABB getAABB() const override;

private:
    BVH();
    AABB aabb;
    std::shared_ptr<IntersectionIF> left;
    std::shared_ptr<IntersectionIF> right;
};
