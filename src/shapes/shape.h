#pragma once

#include "intersection.h"

class Shape: public IntersectionIF
{
public:
    virtual void getIsecData(IntersectionData &isec) const = 0;
    virtual void getNormal(IntersectionData &isec) const = 0;
    virtual void getUV(IntersectionData &isec) const = 0;

    virtual ~Shape(){};
};




