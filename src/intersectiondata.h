#pragma once

#include "vector.h"

class Object;
class Primitive;
class Material;

struct IntersectionData
{
    IntersectionData();
    float tnear;
    size_t idx;
    const Primitive * primitive;
    const Object * object;
    Vector3 phit;
    Vector3 normal;
    const Material * material;
    Vector2 uv;
    Vector3 albedo;
};
