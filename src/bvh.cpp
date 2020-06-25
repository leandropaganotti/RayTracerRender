#include "bvh.h"
#include "invisible.h"
#include <algorithm>

BVH::BVH()
{
    left = right = nullptr;
}

BVH::~BVH(){ }

std::shared_ptr<IntersectionIF> BVH::Create(std::vector<std::shared_ptr<IntersectionIF> > &shapes, size_t l, size_t r)
{
    if (l==r) return shapes[l];

    AABB aabb;
    for (size_t i = l; i <= r; ++i)
        aabb.extend(shapes[i]->getAABB());

    int axis = aabb.getMaxExtent();
    sort(shapes.begin()+l,shapes.begin()+r+1,[axis](const std::shared_ptr<IntersectionIF> &a,const std::shared_ptr<IntersectionIF> &b){
        return a->getAABB().getCenter()[axis] < b->getAABB().getCenter()[axis];
    });

    size_t m = l+(r-l)/2;
    BVH *bvh = new BVH;
    bvh->aabb = aabb;
    bvh->left = Create(shapes, l, m);
    bvh->right = Create(shapes, m+1, r);

    return std::shared_ptr<IntersectionIF>(bvh);
}

AABB BVH::getAABB() const
{
    return aabb;
}

bool BVH::intersection(const Ray &ray, IntersectionData &isec) const
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

bool BVH::intersection(const Ray &ray) const
{
    if(!aabb.intersection(ray)) return false;

    if ( left->intersection(ray) ) return true;
    return right->intersection(ray);
}
