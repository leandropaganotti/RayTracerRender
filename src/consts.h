#ifndef CONSTS_H
#define CONSTS_H
#include "vector.h"

enum class Shader
{
    PHONG,
    GI,         // Global Ilumination: Indirect Light
    GI_DIRECT   // Global Ilumination: Direct + Indirect Light
};

namespace Color {    
    const Vector3 WHITE(1.0f);
    const Vector3 BLACK(0.0f);
    const Vector3 RED(1.0f, 0.0f, 0.0f);
    const Vector3 GREEN(0.0f, 1.0f, 0.0f);
    const Vector3 BLUE(0.0f, 0.0f, 1.0f);
    const Vector3 YELLOW(1.0f, 1.0f, 0.0f);
    const Vector3 CYAN(0.0f, 0.1f, 1.0f);
    const Vector3 MAGENTA(1.0f, 0.0f, 1.0f);
    const Vector3 PURPLE(0.5f, 0.0f, 0.5f);
    const Vector3 ORANGE(1.0f, 0.65f, 0.0f);
    const Vector3 GRAY(0.5f, 0.5f, 0.5f);
}

namespace Vector {
    const Vector3 ZERO(0.0f);
    const Vector3 ORIGIN(0.0f);
    const Vector3 UP(0.0f, 1.0f, 0.0f);
    const Vector3 DOWN(0.0f, -1.0f, 0.0f);
    const Vector3 LEFT(-1.0f, 0.0f, 0.0f);
    const Vector3 RIGHT(1.0f, 0.0f, 0.0f);
    const Vector3 FRONTWARD(0.0f, 0.0f, -1.0f);
    const Vector3 BACKWARD(0.0f, 0.0f, 1.0f);
}

#endif // CONSTS_H
