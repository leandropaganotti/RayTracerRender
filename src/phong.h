#pragma once
#include "raytracer.h"

class Phong: public RayTracer
{
    Vector3 Li(const Ray &ray, const uint8_t depth, float E) override;
    Vector3 phongShading(const Ray &ray, const IntersectionData &isec);
};
