#include "shape.h"
#include "material.h"
#include "ray.h"

const Material *Shape::getMaterial(size_t) const
{
    return material::DiffuseWhite.get();
}

void Shape::getUV(IntersectionData &) const
{
}
