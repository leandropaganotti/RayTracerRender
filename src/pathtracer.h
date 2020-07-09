#pragma once

#include "raytracer.h"

class PathTracer: public RayTracer
{
   Vector3 Li(const Ray &ray, const uint8_t depth, const float E) override;
};

class PathTracerWithDirectSampling: public RayTracer
{
   Vector3 Li(const Ray &ray, const uint8_t depth, const float E) override;
};
