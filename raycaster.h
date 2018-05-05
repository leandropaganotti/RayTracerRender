#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "vector.h"
#include "ray.h"
#include "scene.h"

class RayCaster
{
public:
    RayCaster() = default;

    Vector3f cast(const Ray& ray, const Scene& scene);

    void cast(const Ray& ray, const ObjectVector& objects, IntersectionData& isec);

    bool castShadowRay(const Ray& ray, const ObjectVector& objects, float tMax);

};

#endif // RAYCASTER_H
