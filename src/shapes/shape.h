#pragma once

#include "intersection.h"

class Shape: public IntersectionIF
{
public:
    virtual void            getIsecData(const Ray &ray, IntersectionData &isec) const {};
    virtual void            getNormal(IntersectionData &isec) const = 0;
    virtual void            getUV(IntersectionData &) const;

    virtual const Material* getMaterial(size_t) const;

    virtual ~Shape(){};
};




