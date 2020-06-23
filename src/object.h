#pragma once

#include "shape.h"
#include <memory>
#include "material.h"
#include "aabb.h"

class Object: public Shape
{
    Object(const std::shared_ptr<Shape> &shape):shape(shape){}
    virtual ~Object(){}
    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {

    }
    bool intersection(const Ray &ray) const override
    {
    }
    AABB getAABB() const override
    {
    }
    void getIsecData(const Ray &ray, IntersectionData &isec) const override
    {
    }
    void getNormal(IntersectionData &isec) const override
    {
    }
    void getUV(IntersectionData &) const override
    {
    }
    const Material *getMaterial(size_t) const override
    {
    }
protected:
    std::shared_ptr<Shape> shape;
};
