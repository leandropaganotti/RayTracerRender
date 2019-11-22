#ifndef BOX_H
#define BOX_H

#include "object.h"
#include "matrix.h"
#include "transformation.h"
#include "texture.h"

class GEOMBox : public virtual Shape, public Transformation
{
public:
    GEOMBox() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const;
    bool intersection(const Ray &ray, float &tnear) const;
    const Vector3 normal(const Vector3 &, size_t idx) const;

private:
    static Vector3 min;
    static Vector3 max;
};

class Box: public Object, public GEOMBox
{
    // Object interface
public:
    const std::pair<float, float> uv(const Vector3 &phit, size_t idx) const;
    const Vector3 color(const IntersectionData &isec) const;
};

inline
const std::pair<float, float> Box::uv(const Vector3 &phit, size_t idx) const
{
    float u=0, v=0;
    Vector3 p = getInverse() * phit;

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

inline
const Vector3 Box::color(const IntersectionData &isec) const
{
    const std::pair<float, float> _uv = uv(isec.phit, isec.idx);
    return tex->get(_uv.first, _uv.second) * material.kd;
}



#endif // BOX_H
