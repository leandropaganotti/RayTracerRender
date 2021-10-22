#include "objectvector.h"
#include "object.h"

bool ObjectVector::intersection(const Ray &ray, IntersectionData &isec) const
{
    if(!aabb.intersection(ray)) return false;

    float tmax = ray.tmax;
    for (const auto &o: objects)
    {
        if(o->intersection(ray, isec))
        {
            ray.tmax = isec.tnear;
        }
    }
    if(ray.tmax < tmax) return true;
    return false;
}

bool ObjectVector::intersection(const Ray &ray) const
{
    if(!aabb.intersection(ray)) return false;

    for (const auto &o: objects)
    {
        if(o->intersection(ray)) return true;
    }
    return false;
}

AABB ObjectVector::getAABB() const
{
    return aabb;
}

void ObjectVector::build(const std::vector<std::shared_ptr<Object> > &objs)
{
    destroy();
    for (auto &o: objs)
    {
        objects.push_back(o);
        aabb.extend(o->getAABB());
    }
}

void ObjectVector::build(const std::vector<std::shared_ptr<Primitive> > &shapes)
{
    destroy();
    for (auto &s: shapes)
    {
        objects.push_back(s);
        aabb.extend(s->getAABB());
    }
}

void ObjectVector::destroy()
{
    aabb.setMin(0);
    aabb.setMax(0);
    objects.clear();
}
