#pragma once

#include <vector>
#include <memory>
#include "aggregate.h"
#include "aabb.h"
#include "invisible.h"

class BVHNode: public Intersection
{
public:
    BVHNode() = default;
    ~BVHNode() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    AABB getAABB() const override;

public:
    AABB aabb;
    std::shared_ptr<Intersection> left = nullptr;
    std::shared_ptr<Intersection> right = nullptr;
};

class BVH: public Aggregate
{
public:
    BVH() = default;
    BVH(const BVH&) = delete;
    BVH(const BVH&&) = delete;
    BVH& operator=(const BVH&) = delete;
    BVH& operator=(const BVH&&) = delete;

    ~BVH() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    AABB getAABB() const override;

    void build(const std::vector<std::shared_ptr<Shape> > &) override;
    void build(const std::vector<std::shared_ptr<Object> > &) override;

    void destroy() override;

private:
    std::shared_ptr<Intersection> build(std::vector<std::shared_ptr<Intersection> > &shapes, size_t l, size_t r);

private:
    std::shared_ptr<Intersection> root = shape::Invisible;
};


