#ifndef CONSTS_H
#define CONSTS_H
#include "vector.h"

enum class RayTracerType
{
    RayTracerPhong,
    PathTracer,                 // Global Ilumination: Indirect Light
    PathTracerWithDirectLight   // Global Ilumination: Direct + Indirect Light
};

namespace Vector {
    const Vector3 ZERO(0.0f);
    const Vector3 ORIGIN(0.0f);
    const Vector3 UP(0.0f, 1.0f, 0.0f);
    const Vector3 DOWN(0.0f, -1.0f, 0.0f);
    const Vector3 LEFT(-1.0f, 0.0f, 0.0f);
    const Vector3 RIGHT(1.0f, 0.0f, 0.0f);
    const Vector3 FRONT(0.0f, 0.0f, -1.0f);
    const Vector3 BACK(0.0f, 0.0f, 1.0f);
}

#endif // CONSTS_H
