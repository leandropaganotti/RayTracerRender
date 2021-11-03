#pragma once

#include "vector.h"
#include "primitive.h"
#include "material.h"

class Object;

struct IntersectionData
{
    IntersectionData();
    float tnear;
    size_t idx;
    const Primitive * primitive;
    const Object * object;
    Vector3 phit;
    Vector3 normal;
    Vector3 wo;
    const Material * material;
    Vector2 uv;
    Vector3 albedo;
    float bias = 0;

    Vector3 emittance()
    {
        return material->E;
    }
    void bsdf(ScatterData &srec)
    {
        material->bsdf(*this, srec);
    }
    Vector3 bsdf(const Vector3 & wi)
    {
        return material->bsdf(*this, wi);
    }
};
