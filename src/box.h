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

    bool intersection(const Ray &ray, IntersectionData &isec) const;
    bool intersection(const Ray &ray, float &tnear) const;
    const Vector3f normal(const Vector3f &, size_t idx) const;

private:
    const std::pair<float, float> texUV(const Vector3f &phit, size_t idx) const;

private:
    Vector3f min;
    Vector3f max;    
};

#endif // BOX_H
