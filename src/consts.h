#ifndef CONSTS_H
#define CONSTS_H
#include "vector.h"

enum class RayTracerType
{
    RayTracerPhong,
    PathTracer,                 // Global Ilumination: Indirect Light
    PathTracerWithDirectLight   // Global Ilumination: Direct + Indirect Light
};
#endif // CONSTS_H
