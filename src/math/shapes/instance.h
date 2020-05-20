#pragma once

#include <memory>
#include <float.h>
#include "intersection.h"
#include "matrix.h"
#include "ray.h"
#include "invisible.h"

class Instance: public ShapeNormalUV
{
public:
    Instance(std::shared_ptr<ShapeNormalUV> shape)
    {
        this->shape = shape ? shape : InvisibleShape::GetInstance();
    }
    virtual ~Instance() override {}

    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override
    {
        Ray ray_local = inverse * ray;
        IntersectionData isec_local;
        if (shape->intersection(ray_local, FLT_MAX, isec_local))
        {
            Vector3 phit_local = ray_local.origin + isec_local.tnear * ray_local.direction;
            Vector3 phit = model * phit_local;
            float tnear = (phit - ray.origin).length();
            if(tnear < tmax)
            {
                isec.idx = isec_local.idx;
                isec.tnear = tnear;
                isec.phit = phit;
                isec.phit_local = phit_local;
                return true;
            }
        }
        return false;
    }
    bool intersection(const Ray &ray, float tmax) const override
    {
        Ray ray_local = inverse * ray;
        IntersectionData isec_local;
        if (shape->intersection(ray_local, FLT_MAX, isec_local))
        {
            Vector3 phit_local = ray_local.origin + isec_local.tnear * ray_local.direction;
            Vector3 phit = model * phit_local;
            float tnear = (phit - ray.origin).length();
            if(tnear < tmax)
            {
                return true;
            }
        }
        return false;
    }
    Vector3 getNormal(const Vector3 &phit, size_t idx) const override
    {
        return (inverseTranspose * shape->getNormal(inverse * phit, idx)).normalize();
    }
    Vector2 getUV(const Vector3 &phit, size_t idx) const override
    {
        return shape->getUV(inverse * phit, idx);
    }
    virtual void fetchData(const Ray &, IntersectionData &isec) const override
    {
        isec.normal = (inverseTranspose * shape->getNormal(isec.phit_local, isec.idx)).normalize();
    }
    AABB getAABB() const override
    {
        return AABB(model * shape->getAABB().getMin(), model * shape->getAABB().getMax());
    }

    void setTransformation(const Matrix4 &transformation)
    {
        model = transformation;
        inverse = model.getInverse();
        inverseTranspose = inverse.getTranspose();
    }

protected:
    std::shared_ptr<ShapeNormalUV> shape;

    Matrix4 model;              // object-to-world
    Matrix4 inverse;            // world-to-object
    Matrix4 inverseTranspose;   // matrix for normals transformation
};
