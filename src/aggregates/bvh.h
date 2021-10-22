#pragma once

#include <algorithm>
#include <vector>
#include <memory>
#include "aggregate.h"
#include "aabb.h"
#include "invisible.h"

class BVHNode: public Shape
{
public:
    BVHNode() = default;
    ~BVHNode() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray &ray) const override;
    AABB getAABB() const override;

public:
    AABB aabb;
    std::shared_ptr<Shape> left = nullptr;
    std::shared_ptr<Shape> right = nullptr;
};

template <class T>
class BVH: public Aggregate<T>
{
public:
    BVH() = default;
    BVH(const BVH&) = delete;
    BVH(const BVH&&) = delete;
    BVH& operator=(const BVH&) = delete;
    BVH& operator=(const BVH&&) = delete;

    ~BVH() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        return root->intersection(ray, isec);
    }
    bool intersection(const Ray &ray) const override
    {
        return root->intersection(ray);
    }
    AABB getAABB() const override
    {
        return root->getAABB();
    }

    void build() override
    {
        std::vector<std::shared_ptr<Shape>> leaves;
        for (auto &s: this->shapes)
            leaves.push_back(s);
        root = build(leaves, 0, leaves.size()-1);
    }
    void clear() override
    {
        Aggregate<T>::clear();
        root = primitives::Invisible;
    }

private:
    std::shared_ptr<Shape> build(std::vector<std::shared_ptr<Shape> > &shapes, size_t l, size_t r)
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

private:
    std::shared_ptr<Shape> root = primitives::Invisible;
};


