#include "intersectiondata.h"
#include "primitive.h"
#include "material.h"

IntersectionData::IntersectionData()
{
    tnear = INFINITY;
    primitive = nullptr;
    object = nullptr;
    material = nullptr;
}
