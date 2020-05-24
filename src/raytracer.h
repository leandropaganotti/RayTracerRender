#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "image.h"
#include "camera.h"
#include "scene.h"
#include <functional>

class RayTracer
{
public:    
    RayTracer();
    ~RayTracer();
    void render(const Scene& scene);

    Camera&       getCamera() { return camera; }
    const Camera& getCamera() const { return camera; }
    const Image&  getBuffer() const { return buffer; }    

    void setTracer(RayTracerType type);

private:
    Vector3 rayDirection(float i, float j) const;
    bool    castRay(const Ray &ray, IntersectionData &isec);
    float   castShadowRay(const Ray &ray, float tmax);

    Vector3 phongShading(const Ray &ray, const IntersectionData &isec);
    Vector3 specularReflection(const Ray &ray, const uint8_t depth, const IntersectionData &isec);
    Vector3 transparentMaterial(const Ray &ray, const uint8_t depth, const IntersectionData &isec, const float E=1.0f);

    Vector3 rayTracer(const Ray &ray, const uint8_t depth, const float E=1.0f);
    Vector3 pathTracer(const Ray &ray, const uint8_t depth, const float E=1.0f);
    Vector3 pathTracer2(const Ray &ray, const uint8_t depth, const float E=1.0f);

private:
    Camera camera;
    Image  buffer;
    std::function<Vector3(const Ray &ray, const uint8_t depth, const float E)> tracer;

    const Scene *scene;
};

#endif // RAYTRACER_H
