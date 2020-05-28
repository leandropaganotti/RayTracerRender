#pragma once

#include "image.h"
#include "camera.h"
#include "scene.h"
#include "renderoptions.h"

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
    Vector3 rayDirection(float i, float j) const;
    bool    castRay(const Ray &ray, IntersectionData &isec);
    float   castShadowRay(const Ray &ray, float tmax);

    Vector3 specularReflection(const Ray &ray, const uint8_t depth, const IntersectionData &isec);
    Vector3 transparentMaterial(const Ray &ray, const uint8_t depth, const IntersectionData &isec, float E);

protected:
    RenderOptions renderOptions;
    Camera camera;
    Image  buffer;
    const Scene *scene;
    virtual Vector3 trace(const Ray &ray, const uint8_t depth, float E) = 0;
};

class Minimum: public RayTracer
{
    Vector3 trace(const Ray &ray, const uint8_t depth, const float E) override;
};
