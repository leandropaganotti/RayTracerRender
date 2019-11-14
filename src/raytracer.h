#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "image.h"
#include "camera.h"
#include "scene.h"

class RayTracer
{
public:    
    RayTracer();

    void render(const Scene& scene);

    Camera&       getCamera() { return camera; }
    const Camera& getCamera() const { return camera; }
    const Image&  getBuffer() const { return buffer; }    

    void setTracer(Shader type);
private:
    Vector3 rayDirection(float i, float j) const;
    Vector3 castRay(const Ray &ray, const Scene &scene, const uint8_t depth, float E=1.0f);
    bool    closestIntersection(const Ray &ray, const ObjectVector &objects, IntersectionData &isec);
    float   castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax);

    Vector3 diffusePhongReflection(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec, float E=1.0f);
    Vector3 specularReflection(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E=1.0f);
    Vector3 transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E=1.0f);

    Vector3 phongIllumination(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E=1.0f);
    Vector3 globalIllumination(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E=1.0f);

    Vector3 pathTracer(const Ray &ray, const Scene &scene, const uint8_t depth);
    Vector3 phong(const Ray &ray, const Scene &scene, const uint8_t depth);

private:
    Camera camera;
    Image  buffer;
    std::function<Vector3(const Ray &ray, const Scene &scene, const uint8_t depth)> tracer;


};

#endif // RAYTRACER_H
