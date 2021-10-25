#pragma once

#include <memory>
#include "renderoptions.h"
#include "sampler2d.h"
#include "scene.h"
#include "image.h"
#include "intersectiondata.h"

class Camera;

class RayTracer
{
public:
    static std::unique_ptr<RayTracer> Create(const RenderOptions& renderOptions);
    std::shared_ptr<Image> render(const Scene& scene, const Camera &camera);

    virtual ~RayTracer() = default;

protected:
    RayTracer(const RenderOptions& renderOptions);

    Vector3 transparentMaterial(const Ray &ray, const uint8_t depth, const IntersectionData &isec, float E);

protected:
    RenderOptions renderOptions;
    const Scene *scene;
    Jitter rng;
    TentFilter filter;

    virtual Vector3 trace(const Ray &ray, const uint8_t depth, float E) = 0;
};

class Minimum: public RayTracer
{
public:
    Minimum(const RenderOptions& renderOptions);
private:
    Vector3 trace(const Ray &ray, const uint8_t depth, const float E) override;
};
