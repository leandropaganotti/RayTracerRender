#include "invisible.h"

InvisibleShape::InvisibleShape()
{

}

bool InvisibleShape::intersection(const Ray &, float, IntersectionData &) const
{
    return false;
}
bool InvisibleShape::intersection(const Ray &, float) const
{
    return false;
}
Vector3 InvisibleShape::normal(const Vector3 &, size_t) const
{
    return {0,0,0};
}
Vector2 InvisibleShape::uv(const Vector3 &, size_t) const
{
    return Vector2 (0,0);
}
void InvisibleShape::fetch(const Ray &, IntersectionData &) const
{
}
