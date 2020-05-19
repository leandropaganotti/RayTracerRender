#include "bvh.h"


BVH::BVH()
{
    left = right = nullptr;
}

BVH::~BVH(){ }

std::shared_ptr<Shape> BVH::Create(std::vector<std::shared_ptr<Shape>> &shapes)
{
    if (shapes.size() == 0) return InvisibleShape::GetInstance();

    std::vector<std::shared_ptr<Shape>> shapes_;
    shapes_.push_back(std::shared_ptr<Shape>());
    for (auto s: shapes)
        shapes_.push_back(s);

    return Create(shapes_, 1, shapes_.size()-1, 0);
}

std::shared_ptr<Shape> BVH::Create(std::vector<std::shared_ptr<Shape> > &shapes, size_t l, size_t r, size_t axis)
{
    if (l==r)
    {
        return shapes[l];
    }

    AABB aabb;
    for (size_t i = l; i <= r; ++i)
    {
        aabb.extend(shapes[i]->getAABB());
    }
    auto bvh = std::shared_ptr<BVH>(new BVH);
    bvh->aabb = aabb;
    if(l == (r-1))
    {
        bvh->left = shapes[l];
        bvh->right = shapes[r];
        return bvh;
    }
    size_t pivot = qsplit(shapes, l, r, aabb.getCenter()[axis], axis);
    bvh->left = Create(shapes, l, pivot-1, (axis+1)%3);
    bvh->right = Create(shapes, pivot, r, (axis+1)%3);

    return bvh;
}

Vector3 BVH::normal(const Vector3 &phit, size_t idx) const
{

}

Vector2 BVH::uv(const Vector3 &phit, size_t idx) const
{

}

void BVH::fetch(const Ray &ray, IntersectionData &isec) const
{

}

AABB BVH::getAABB() const
{
    return aabb;
}

bool BVH::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
{
    if(!aabb.intersection(ray, tmax)) return false;

    bool hit_left = left->intersection(ray, tmax, isec);
    if ( hit_left )
    {
        tmax = isec.tnear;
    }

    bool hit_right = right->intersection(ray, tmax, isec);
    return hit_left || hit_right;
}

bool BVH::intersection(const Ray &ray, float tmax) const
{
    if(!aabb.intersection(ray, tmax)) return false;

    if ( left->intersection(ray, tmax) ) return true;
    return right->intersection(ray, tmax);
}

size_t BVH::qsplit(std::vector<std::shared_ptr<Shape> > &shapes, size_t l, size_t r, float pivot, size_t axis)
{
    size_t j = l-1;
    for(size_t i=l; i <= r; ++i)
    {
        float center = shapes[i]->getAABB().getCenter()[axis];
        if (center < pivot)
        {
            ++j;
            auto tmp = shapes[i];
            shapes[i] = shapes[j];
            shapes[j] = tmp;
        }
    }
    if(j==(l-1) || j==r) return (l+r)/2;
    return j+1;
}
