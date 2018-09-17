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

    Vector3f phongReflection(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec);
    Vector3f specularMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);
    Vector3f specularReflection(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);
    Vector3f transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);

    Vector3f trace(const Ray &ray, const Scene &scene, const uint8_t depth);
    Vector3f globalIlumination(const Scene &scene);
    Vector3f diffuseReflection_GI(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);

public:

    Render() = default;

    Camera& getCamera();
    const Image& getImage() const;

    void render(const Scene &scene);
};

#endif // RENDER_H
