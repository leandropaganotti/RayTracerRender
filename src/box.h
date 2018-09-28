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
    const Vector3 normal(const Vector3 &, size_t idx) const;

private:
    const std::pair<float, float> texUV(const Vector3 &phit, size_t idx) const;

private:
    Vector3 min;
    Vector3 max;    
};

#endif // BOX_H
