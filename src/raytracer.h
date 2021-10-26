#pragma once

#include <memory>
#include "renderoptions.h"
#include "sampler2d.h"
#include "scene.h"
#include "image.h"
#include "intersectiondata.h"
#include "camera.h"

class RayTracer
{
public:
    virtual ~RayTracer() = default;

    static std::unique_ptr<RayTracer> Create(const RenderOptions& renderOptions);
    std::shared_ptr<Image> render(const Scene& scene);

    Camera& getCamera();
    void setCameraOptions(const CameraOptions& options);

protected:
    RayTracer(const RenderOptions& renderOptions);
    Vector3 transparentMaterial(const Ray &ray, const uint8_t depth, const IntersectionData &isec, float E);
    virtual Vector3 trace(const Ray &ray, const uint8_t depth, float E) = 0;

protected:
    RenderOptions renderOptions;
    Camera camera;
    const Scene *scene;
    Jitter sampler2d;
    TentFilter filter;
};

class Minimum: public RayTracer
{
public:
    Minimum(const RenderOptions& renderOptions);
private:
    Vector3 trace(const Ray &ray, const uint8_t depth, const float E) override;
};
