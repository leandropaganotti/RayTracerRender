#pragma once

#include "raytracer.h"

class PathTracer: public RayTracer
{
public:
   PathTracer(const RenderOptions& renderOptions);
private:
   Vector3 trace(const Ray &ray, const uint8_t depth) override;
};
