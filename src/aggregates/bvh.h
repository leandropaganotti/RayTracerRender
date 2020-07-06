#pragma once

#include <vector>
#include <memory>
#include "aggregate.h"
#include "aabb.h"

class BVHNode: public IntersectionIF
{
public:
    BVHNode();
    ~BVHNode();
    static std::shared_ptr<IntersectionIF> Create(std::vector<std::shared_ptr<IntersectionIF> > &shapes, size_t l, size_t r);

    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;

    AABB getAABB() const override;

private:
    AABB aabb;
    std::shared_ptr<IntersectionIF> left;
    std::shared_ptr<IntersectionIF> right;
};

class BVH: public Aggregate
{
public:
    BVH();
    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    AABB getAABB() const override;
    void create(const std::vector<std::shared_ptr<Object> > &) override;
    void create(const std::vector<std::shared_ptr<Shape> > &) override;
    void destroy() override;
protected:
    std::shared_ptr<IntersectionIF> root;
};


