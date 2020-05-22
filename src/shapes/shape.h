#pragma once

#include "intersection.h"

class Shape: public IntersectionIF
{
public:
    virtual Vector3         getNormal(const Vector3 &phit, size_t idx) const = 0;
    virtual void            getIsecData(const Ray &ray, IntersectionData &isec) const;
    virtual const Material* getMaterial(size_t) const;
    virtual Vector2         getUV(const Vector3 &, size_t) const;

    virtual ~Shape(){};
};




