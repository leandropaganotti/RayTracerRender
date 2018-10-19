#ifndef RENDER_H
#define RENDER_H

#include "image.h"
#include "camera.h"
#include "scene.h"

class Render: public Camera
{
    Vector3 castRay(const Ray &ray, const Scene &scene, const uint8_t depth, float E=1.0f);

    bool    closestIntersection(const Ray &ray, const ObjectVector &objects, IntersectionData &isec);
    float   castShadowRay(const Ray &ray, const ObjectVector &objects, float tMax);

    Vector3 diffusePhongReflection(const Ray &ray, const Scene &scene, const uint8_t, const IntersectionData &isec, float E=1.0f);
    Vector3 specularReflection(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E=1.0f);
    Vector3 transparentMaterial(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E=1.0f);

    Vector3 phongIllumination(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E=1.0f);
    Vector3 globalIllumination(const Ray &ray, const Scene &scene, const uint8_t depth, const IntersectionData &isec, float E=1.0f);

public:
    /*
     * This is Ray Tracer core algorithm
    */
    void capture(const Scene &scene);
};

#endif // RENDER_H
