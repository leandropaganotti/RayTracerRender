#ifndef RENDER_H
#define RENDER_H

#include "image.h"
#include "camera.h"
#include "scene.h"

#define NRAYS 1
#define NTHREADS 4

class Render
{
    Camera _camera;
    Image  _frame;

    const float bias = 0.0001;

    void render_1x1x1(const Scene *scene, size_t start, size_t end);

    Vector3f trace(const Ray &ray, const Scene &scene);

    void castRay(const Ray &ray, const ObjectVector &objects, IntersectionData &isec);
    bool castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax);

public:

    Render() = default;

    Camera& camera() { return _camera; }
    Image& frame()   { return _frame;  }

    void render(const Scene &scene, size_t width, size_t height, uint8_t nrays=NRAYS, uint8_t nthreads=NTHREADS);

};

#endif // RENDER_H
