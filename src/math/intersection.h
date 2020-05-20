#pragma once

#include "vector.h"

class ShapeNormalUV;
class Ray;
class Material;

struct IntersectionData
{
    float tnear;
    size_t idx;
    const ShapeNormalUV * shape;
    Vector3 phit;
    Vector3 phit_local;
    Vector3 normal;
    const Material * material;
    Vector2 uv;
    Vector3 color;
};
