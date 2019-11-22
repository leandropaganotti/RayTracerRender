#ifndef SHAPE_H
#define SHAPE_H

#include <stdio.h>
#include "intersection.h"

class Vector3;

class Shape: public IntersectionInt
{
public:    
    virtual ~Shape() {}
    virtual const Vector3 normal(const Vector3 &phit, size_t idx) const = 0;    
};


#endif // SHAPE_H
