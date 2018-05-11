#ifndef RENDER_H
#define RENDER_H

#include "image.h"
#include "camera.h"
#include "scene.h"

#define NRAYS 1
#define NTHREADS 4
#define MAX_DEPTH 5

class Render
{
    Camera camera;
    Image  image;

    const float bias = 0.0001;

    void renderSingleThread(const Scene *scene, size_t start, size_t end, size_t nrays);

    Vector3f trace(const Ray &ray, const Scene &scene, const uint8_t depth);

    void castRay(const Ray &ray, const ObjectVector &objects, IntersectionData &isec);
    bool castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax);

public:

    Render();

    Camera& getCamera() { return camera; }
    Image& getImage()   { return image;  }

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
