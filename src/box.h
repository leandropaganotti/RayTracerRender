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
    Vector3f min;
    Vector3f max;    

private:
    const std::pair<float, float> texUV(const Vector3f &phit, size_t idx) const;
};

inline
const std::pair<float, float> Box::texUV(const Vector3f &phit, size_t idx) const
{
    float u=0, v=0;
    Vector3f p = inverse * phit;

    if (idx == 1 || idx == 2)
    {
        u = p.z; v = p.y;
    } else if (idx == 3 || idx == 4)
    {
        u = p.x; v = p.z;
    } else
    {
        u = p.x; v = p.y;
    }

    return std::make_pair(u, v);
}

#endif // BOX_H
