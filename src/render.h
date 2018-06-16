#ifndef RENDER_H
#define RENDER_H

#include "image.h"
#include "camera.h"
#include "scene.h"

#define NRAYS 1
#define NTHREADS 4
#define MAX_DEPTH 3

class Render: public Camera
{
    const float bias = 0.001;

    void renderSingleThread(const Scene *scene, size_t startRow, size_t endRow, size_t nrays);

    Vector3f rayTrace(const Ray &ray, const Scene &scene, const uint8_t depth);

    void castRay(const Ray &ray, const ObjectVector &objects, IntersectionData &isec);
    bool castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax);

    Vector3f diffuseMaterial(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec);
    Vector3f specularMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);
    Vector3f mirrorMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);
    Vector3f transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec);

public:

    Render() = default;

    void setCameraOptions(const CameraOptions &options);

    Camera& getCamera();
    const Image& getImage() const;

    /*
    * render the scene with current the CameraOptions
    * */
    void render(const Scene &scene, uint8_t nrays=NRAYS, uint8_t nthreads=NTHREADS);

    /*
     * render the scene with current the CameraOptions, but set resolution width x height
     * */
    void render(size_t width, size_t height, const Scene &scene, uint8_t nrays=NRAYS, uint8_t nthreads=NTHREADS);

    /*
     * render the scene with specified CameraOptions opts
     * */
    void render(const CameraOptions &opts, const Scene &scene, uint8_t nrays=NRAYS, uint8_t nthreads=NTHREADS);

};

#endif // RENDER_H
