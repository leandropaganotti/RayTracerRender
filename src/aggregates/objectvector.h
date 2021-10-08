#pragma once

#include "aggregate.h"
#include "aabb.h"
#include "ray.h"

class ObjectVector: public Aggregate
{
public:
    ObjectVector() = delete;
    ~ObjectVector() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    AABB getAABB() const override;

    void build(const std::vector<std::shared_ptr<Shape>> &shapes) override;
    void build(const std::vector<std::shared_ptr<Object>> &objects) override;
    void destroy() override;

protected:
    AABB aabb;
    std::vector<std::shared_ptr<Intersection>> objects;
};
