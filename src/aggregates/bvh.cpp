#include "bvh.h"
#include "invisible.h"
#include "ray.h"
#include "object.h"
#include <algorithm>

BVHNode::BVHNode()
{
    left = right = nullptr;
}

BVHNode::~BVHNode(){ }

std::shared_ptr<IntersectionIF> BVHNode::Create(std::vector<std::shared_ptr<IntersectionIF> > &shapes, size_t l, size_t r)
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
    BVHNode *bvh = new BVHNode;
    bvh->aabb = aabb;
    bvh->left = Create(shapes, l, m);
    bvh->right = Create(shapes, m+1, r);

    return std::shared_ptr<IntersectionIF>(bvh);
}

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

bool BVH::intersection(const Ray &ray, IntersectionData &isec) const
{
    return root->intersection(ray, isec);
}

BVH::BVH()
{
    root = shape::Invisible;
}

bool BVH::intersection(const Ray &ray) const
{
    return root->intersection(ray);
}

AABB BVH::getAABB() const
{
    return root->getAABB();
}

void BVH::create(const std::vector<std::shared_ptr<Object> > &objects)
{
    destroy();
    if (objects.size() == 0) root = shape::Invisible;

    std::vector<std::shared_ptr<IntersectionIF>> leaves;
    for (auto &o: objects)
        leaves.push_back(o);

    root = BVHNode::Create(leaves, 0, leaves.size()-1);
}

void BVH::create(const std::vector<std::shared_ptr<Shape> > &shapes)
{
    destroy();
    if (shapes.size() == 0) root = shape::Invisible;

    std::vector<std::shared_ptr<IntersectionIF>> leaves;
    for (auto &o: shapes)
        leaves.push_back(o);

    root = BVHNode::Create(leaves, 0, leaves.size()-1);
}

void BVH::destroy()
{
    root = shape::Invisible;
}
