#include "invisibleshape.h"

bool InvisibleShape::intersection(const Ray &, IntersectionData &) const
{
    return false;
}
bool InvisibleShape::intersection(const Ray &, float &) const
{
    return false;
}
Vector3 InvisibleShape::normal(const Vector3 &, size_t) const
{
    return {0,0,0};
}
std::pair<float, float> InvisibleShape::uv(const Vector3 &, size_t) const
{
    return std::pair<float, float> (0,0);
}

