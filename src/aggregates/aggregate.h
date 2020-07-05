#pragma once
#include "intersection.h"
#include <memory>
#include <vector>
class Object;

class Aggregate: public IntersectionIF
{
public:
    virtual ~Aggregate(){}
    virtual void create(const std::vector<std::shared_ptr<Object>> &) = 0;
    virtual void destroy() = 0;
};
