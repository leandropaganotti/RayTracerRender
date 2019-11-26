#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "vector.h"

class Object;

struct IntersectionData
{
    float tnear;
    size_t idx;
    const Object * object;
    Vector3 phit;
    Vector3 normal;
};

#endif // INTERSECTION_H
