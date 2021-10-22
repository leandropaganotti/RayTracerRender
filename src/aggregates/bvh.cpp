#include "bvh.h"
#include "ray.h"
#include "object.h"

AABB BVHNode::getAABB() const
{
    return aabb;
}

bool BVHNode::intersection(const Ray &ray, IntersectionData &isec) const
{
    if(!aabb.intersection(ray)) return false;

    bool hit_left = left->intersection(ray, isec);
    if ( hit_left )
    {
        ray.tmax = isec.tnear;
    }

    bool hit_right = right->intersection(ray, isec);
    return hit_left || hit_right;
}

bool BVHNode::intersection(const Ray &ray) const
{
    if(!aabb.intersection(ray)) return false;

    if ( left->intersection(ray) ) return true;
    return right->intersection(ray);
}
