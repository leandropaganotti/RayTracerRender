#pragma once
#include "intersection.h"
#include <memory>
#include <vector>

class Aggregate: public IntersectionIF
{
public:
    Aggregate() = default;
    virtual ~Aggregate() = default;
    virtual void build(const std::vector<std::shared_ptr<Shape>> &) = 0;
    virtual void build(const std::vector<std::shared_ptr<Object>> &) = 0;
    virtual void destroy() = 0;
};
