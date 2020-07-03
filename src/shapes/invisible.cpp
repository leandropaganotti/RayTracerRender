#include "invisible.h"
#include "aabb.h"

namespace shape
{
    std::shared_ptr<Shape> Invisible = std::make_shared<InvisibleShape>();
}

bool InvisibleShape::intersection(const Ray &, IntersectionData &) const
{
    return false;
}
bool InvisibleShape::intersection(const Ray &) const
{
    return false;
}
void InvisibleShape::getIsecData(IntersectionData &) const
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
