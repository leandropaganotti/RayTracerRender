#ifndef CONSTS_H
#define CONSTS_H
#include "vector.h"

enum class Shade
{
    PHONG,
    GI,         // Global Ilumination: Indirect Light
    GI_DIRECT   // Global Ilumination: Direct + Indirect Light
};

namespace Color {
    const Vector3f WHITE(1.0f);
    const Vector3f BLACK(0.0f);
}

#endif // CONSTS_H
