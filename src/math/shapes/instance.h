#pragma once

#include <memory>
#include <float.h>
#include "intersection.h"
#include "matrix.h"
#include "ray.h"
#include "invisible.h"

class Instance: public Shape
{
public:
    Instance(std::shared_ptr<Shape> shape)
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
    Vector3 normal(const Vector3 &phit, size_t idx) const override
    {
        return (inverseTranspose * shape->normal(inverse * phit, idx)).normalize();
    }
    Vector2 uv(const Vector3 &phit, size_t idx) const override
    {
        return shape->uv(inverse * phit, idx);
    }
    virtual void fetch(const Ray &, IntersectionData &isec) const override
    {
        isec.normal = (inverseTranspose * shape->normal(isec.phit_local, isec.idx)).normalize();
    }

    void setTransformation(const Matrix4 &transformation)
    {
        model = transformation;
        inverse = model.getInverse();
        inverseTranspose = inverse.getTranspose();
    }

protected:
    std::shared_ptr<Shape> shape;

    Matrix4 model;              // object-to-world
    Matrix4 inverse;            // world-to-object
    Matrix4 inverseTranspose;   // matrix for normals transformation
};
