#pragma once

#include "aggregate.h"
#include "aabb.h"
#include "ray.h"

class ObjectVector: public Aggregate
{
public:
    ObjectVector();
    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    AABB getAABB() const override;
    void create(const std::vector<std::shared_ptr<Object> > &) override;
    void destroy() override;
protected:
    AABB aabb;
    const std::vector<std::shared_ptr<Object>> *objects;
};
