#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "vector.h"
#include "ray.h"
#include "scene.h"

class RayCaster
{
public:
    RayCaster();

    Vector3f cast(const Ray& ray, const Scene& scene);

    void cast(const Ray& ray, const ObjectVector& objects, IntersectionData& inter);

    bool cast(const Ray& ray, const ObjectVector& objects, float distToLight);

};

#endif // RAYCASTER_H
