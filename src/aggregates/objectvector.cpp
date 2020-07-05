#include "objectvector.h"
#include "object.h"

ObjectVector::ObjectVector(){}

bool ObjectVector::intersection(const Ray &ray, IntersectionData &isec) const
{
    //if(!aabb.intersection(ray)) return false;

    for (const auto &o: *objects)
    {
        if(o->intersection(ray, isec))
        {
            ray.tmax = isec.tnear;
        }
    }
    if(isec.object) return true;
    return false;
}

bool ObjectVector::intersection(const Ray &ray) const
{
    //if(!aabb.intersection(ray)) return false;

    for (const auto &o: *objects)
    {
        if(o->intersection(ray)) return true;
    }
    return false;
}

AABB ObjectVector::getAABB() const
{
    return aabb;
}

void ObjectVector::create(const std::vector<std::shared_ptr<Object> > &objs)
{
    destroy();
    objects = &objs;
    for (const auto &o: *objects)
        aabb.extend(o->getAABB());
}

void ObjectVector::destroy()
{
    aabb.setMin(0);
    aabb.setMax(0);
    objects = nullptr;
}
