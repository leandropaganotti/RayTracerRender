#ifndef RENDER_H
#define RENDER_H

#include "image.h"
#include "camera.h"
#include "scene.h"

class Render: public Camera
{
    const float bias = 0.001;

    void renderSingleThread(const Scene *scene, size_t startRow, size_t endRow);

    Vector3f rayTrace(const Ray &ray, const Scene &scene, const uint8_t depth);

    bool castRay(const Ray &ray, const ObjectVector &objects, IntersectionData &isec);
    bool castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax);

    Vector3f diffuseMaterial(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec);
    Vector3f specularMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);
    Vector3f mirrorMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);
    Vector3f transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);

public:

    Render() = default;

    Camera& getCamera();
    const Image& getImage() const;

    /*
    * render the scene with current CameraOptions
    * */
    void render(const Scene &scene);


    void render_omp(const Scene &scene);
};

#endif // RENDER_H
