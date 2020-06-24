#include "invisible.h"
#include "aabb.h"

std::shared_ptr<InvisibleShape> InvisibleShape::instance = std::shared_ptr<InvisibleShape>(new InvisibleShape);

InvisibleShape::InvisibleShape()
{

}

std::shared_ptr<InvisibleShape> InvisibleShape::GetInstance()
{
   return instance;
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
    return AABB();
}
