#include "invisible.h"
#include "aabb.h"

namespace primitives
{
    std::shared_ptr<Primitive> Invisible = std::make_shared<InvisibleShape>();
}

bool InvisibleShape::intersection(const Ray &, IntersectionData &) const
{
    return false;
}
bool InvisibleShape::intersection(const Ray &) const
{
    return false;
}
void InvisibleShape::getNormalAndUV(IntersectionData &) const
{
}

void InvisibleShape::getNormal(IntersectionData&) const
{
}

void InvisibleShape::getUV(IntersectionData &) const
{
}

AABB InvisibleShape::getAABB() const
{
    return AABB(Vector3(0), Vector3(0));
}
