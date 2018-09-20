#ifndef BOX_H
#define BOX_H

#include "object.h"
#include "matrix.h"
#include "transformation.h"
#include "texture.h"

class Box : public Object
{
public:
    Box();
    // Object interface
    bool intersection(const Ray &ray, IntersectionData &isec) const;
    bool intersection(const Ray &ray, float &tnear) const;
    const Vector3f normal(const Vector3f &, size_t idx) const;
    const Vector3f texture(const Vector3f &phit, size_t idx) const;
    std::unique_ptr<Texture>& getTex();

private:
    Vector3f min;
    Vector3f max;

    std::unique_ptr<Texture> tex;
};

#endif // BOX_H
