#include "invisible.h"

std::shared_ptr<InvisibleShape> InvisibleShape::instance = std::shared_ptr<InvisibleShape>(new InvisibleShape);

InvisibleShape::InvisibleShape()
{

}

std::shared_ptr<InvisibleShape> InvisibleShape::GetInstance()
{
   return instance;
}

bool InvisibleShape::intersection(const Ray &, float, IntersectionData &) const
{
    return false;
}
bool InvisibleShape::intersection(const Ray &, float) const
{
    return false;
}
void InvisibleShape::getIsecData(const Ray &, IntersectionData &) const
{
}

Vector3 InvisibleShape::getNormal(const Vector3 &, size_t ) const
{
    return Vector3(0);
}

Vector2 InvisibleShape::getUV(const Vector3 &, size_t ) const
{
    return Vector2(0);
}

AABB InvisibleShape::getAABB() const
{
    return AABB();
}
