#pragma once
#include "vector.h"

using Color = Vector3;

namespace color
{
    const Color WHITE(1.0f);
    const Color BLACK(0.0f);
    const Color RED(1.0f, 0.0f, 0.0f);
    const Color GREEN(0.0f, 1.0f, 0.0f);
    const Color BLUE(0.0f, 0.0f, 1.0f);
    const Color YELLOW(1.0f, 1.0f, 0.0f);
    const Color CYAN(0.0f, 0.1f, 1.0f);
    const Color MAGENTA(1.0f, 0.0f, 1.0f);
    const Color PURPLE(0.5f, 0.0f, 0.5f);
    const Color ORANGE(1.0f, 0.65f, 0.0f);
    const Color GRAY(0.5f, 0.5f, 0.5f);
}
