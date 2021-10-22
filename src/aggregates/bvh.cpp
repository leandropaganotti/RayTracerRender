#include "bvh.h"
#include "ray.h"
#include "object.h"
#include <algorithm>

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

bool BVH::intersection(const Ray &ray) const
{
    return root->intersection(ray);
}

AABB BVH::getAABB() const
{
    return root->getAABB();
}

void BVH::build(const std::vector<std::shared_ptr<Primitive> > &shapes)
{
    destroy();

    std::vector<std::shared_ptr<Shape>> leaves;
    for (auto &s: shapes)
        leaves.push_back(s);

    root = build(leaves, 0, leaves.size()-1);
}

void BVH::build(const std::vector<std::shared_ptr<Object> > &objects)
{
    destroy();

    std::vector<std::shared_ptr<Shape>> leaves;
    for (auto &o: objects)
        leaves.push_back(o);

    root = build(leaves, 0, leaves.size()-1);
}

void BVH::destroy()
{
    root = primitives::Invisible;
}

std::shared_ptr<Shape> BVH::build(std::vector<std::shared_ptr<Shape> > &shapes, size_t l, size_t r)
{
    if (l>r || shapes.empty() || r>=shapes.size()) return primitives::Invisible;

    if (l==r) return shapes[l];

    AABB aabb;
    for (size_t i = l; i <= r; ++i)
        aabb.extend(shapes[i]->getAABB());

    int axis = aabb.getMaxExtent();
    sort(shapes.begin()+l,shapes.begin()+r+1,[axis](const std::shared_ptr<Shape> &a,const std::shared_ptr<Shape> &b){
        return a->getAABB().getCenter()[axis] < b->getAABB().getCenter()[axis];
    });

    size_t m = l+(r-l)/2;
    auto bvh_node = std::make_shared<BVHNode>();
    bvh_node->aabb  = aabb;
    bvh_node->left  = build(shapes, l, m);
    bvh_node->right = build(shapes, m+1, r);

    return bvh_node;
}
