#include "intersectiondata.h"
#include "shape.h"
#include "material.h"

IntersectionData::IntersectionData()
{
    tnear = INFINITY;
    shape = nullptr;
    object = nullptr;
    material = nullptr;
}
