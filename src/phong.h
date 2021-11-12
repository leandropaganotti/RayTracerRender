#pragma once
#include "raytracer.h"

class Phong: public RayTracer
{
public:
    Phong(const RenderOptions& renderOptions);
private:
    Vector3 trace(const Ray &ray, const uint8_t depth) override;
    Vector3 phongShading(const Ray &ray, const IntersectionData &isec);
};
