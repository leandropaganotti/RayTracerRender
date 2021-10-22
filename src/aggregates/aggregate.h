#pragma once
#include "shape.h"
#include <memory>
#include <vector>

class Primitive;
class Object;

class Aggregate: public Shape
{
public:
    Aggregate() = default;
    virtual ~Aggregate() = default;
    virtual void build(const std::vector<std::shared_ptr<Primitive>> &) = 0;
    virtual void build(const std::vector<std::shared_ptr<Object>> &) = 0;
    virtual void destroy() = 0;
};
