#include "shape.h"
#include "material.h"
#include "ray.h"

void Shape::getIsecData(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = getNormal(isec.phit, isec.idx);
    isec.material = getMaterial(isec.idx);
    isec.uv = getUV(isec.phit, isec.idx);
}

const Material *Shape::getMaterial(size_t) const
{
    return Material::DiffuseWhite.get();
}

Vector2 Shape::getUV(const Vector3 &, size_t) const
{
    return Vector2(0);
}
