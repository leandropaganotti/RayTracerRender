#ifndef PLANE_H
#define PLANE_H

#include "object.h"
#include "texture.h"

class Plane: public Object
{
public:       
    Plane(const Vector3f& O={0.0f}, const Vector3f& N={0.0f, 1.0f, 0.0f}, const Vector3f& color={1.0f});

    bool intersection(const Ray& ray, IntersectionData& isec) const;
    bool intersection(const Ray& ray, float &tnear) const;
    const Vector3f normal(const Vector3f &, size_t) const;
    const Vector3f texture(const Vector3f &phit) const;

    static bool intersection(const Vector3f& O, const Vector3f& n, const Ray& ray, float& tnear);

    Vector3f O;    // a point O(origin) on the plane
    Vector3f N;    // normal to the plane

private:
    std::unique_ptr<Texture> tex;
};

#endif // PLANE_H

