#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"

class Material
{
public:
    enum class Type { DIFFUSE, SPECULAR, TRANSPARENT };
    Type type;
    Vector3 kd;
    Vector3 E;
    float ks;
    float m;
    float R0;
    float index;

    Material()
    {
        kd = 1.0f;
        E = 0.0f;
        ks = 1.0f;
        m = 30.0f;
        R0 = 8.0f;
        index = 1.55f; // refractive index for glass
        type = Type::DIFFUSE;
    }
};

#endif // MATERIAL_H
