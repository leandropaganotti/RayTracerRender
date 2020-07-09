#pragma once

#include "image.h"
#include "camera.h"
#include "scene.h"
#include "renderoptions.h"
#include "sampler2d.h"

class RayTracer
{
public:
    static std::shared_ptr<RayTracer> Create(Illumination illum);

    RayTracer();
    ~RayTracer();

    void render(const Scene& scene);

    Camera&       getCameraRef() { return camera; }
    const Camera& getCameraRef() const { return camera; }
    const Image&  getBuffer() const { return buffer; }    

    void setRenderOptions(const RenderOptions &value);
    void setCameraOptions(const CameraOptions &value);

protected:
    Vector3 transparentMaterial(const Ray &ray, const uint8_t depth, const IntersectionData &isec, float E);

protected:
    RenderOptions renderOptions;
    Camera camera;
    Image  buffer;
    const Scene *scene;
    Jitter rng;
    TentFilter filter;

    virtual Vector3 Li(const Ray &ray, const uint8_t depth, float E) = 0;
};

class Minimum: public RayTracer
{
    Vector3 Li(const Ray &ray, const uint8_t depth, const float E) override;
};
