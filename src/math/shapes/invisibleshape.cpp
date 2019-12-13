#include "invisibleshape.h"

std::shared_ptr<InvisibleShape> InvisibleShape::instance;

std::shared_ptr<InvisibleShape> InvisibleShape::GetInstance()
{
    return instance ? instance : instance = std::shared_ptr<InvisibleShape>(new InvisibleShape());
}

bool InvisibleShape::intersection(const Ray &, IntersectionData &) const
{
    return false;
}
bool InvisibleShape::intersection(const Ray &, float &) const
{
    return false;
}
const Vector3 InvisibleShape::normal(const Vector3 &, size_t) const
{
    return {0,0,0};
}
const std::pair<float, float> InvisibleShape::uv(const Vector3 &, size_t) const
{
    return std::pair<float, float> (0,0);
}

