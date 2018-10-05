#ifndef RENDER_H
#define RENDER_H

#include "image.h"
#include "camera.h"
#include "scene.h"

class Render: public Camera
{
    const float bias = 0.001;

    bool castRay(const Ray &ray, const ObjectVector &objects, IntersectionData &isec);
    bool castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax);

    Vector3 diffusePhongReflection(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec);
    Vector3 specularReflection(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);
    Vector3 transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);

    Vector3 shader(const Ray &ray, const Scene &scene, const uint8_t depth);
    Vector3 pathTrace(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);

public:

    Render() = default;

    Camera& getCamera();
    const Image& getImage() const;

    void render(const Scene &scene);
};

#endif // RENDER_H
