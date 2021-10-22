#pragma once

#include "aggregate.h"
#include "intersectiondata.h"
#include "aabb.h"
#include "ray.h"

template <class T>
class ObjectVector: public Aggregate<T>
{
public:
    ObjectVector() = default;
    ~ObjectVector() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        if(!aabb.intersection(ray)) return false;

        float tmax = ray.tmax;
        for (const auto &s: this->shapes)
        {
            if(s->intersection(ray, isec))
            {
                ray.tmax = isec.tnear;
            }
        }
        if(ray.tmax < tmax) return true;
        return false;
    }
    bool intersection(const Ray &ray) const override
    {
        if(!aabb.intersection(ray)) return false;

        for (const auto &s: this->shapes)
        {
            if(s->intersection(ray)) return true;
        }
        return false;
    }
    AABB getAABB() const override
    {
        return aabb;
    }

    void build() override
    {

    }
    void clear() override
    {
        Aggregate<T>::clear();
        aabb.setMin(0);
        aabb.setMax(0);
    }

protected:
    AABB aabb;
};
