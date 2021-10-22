#pragma once

#include "matrix.h"
#include "primitive.h"
#include "material.h"
#include "invisible.h"
#include "intersectiondata.h"
#include <memory>

class Object: public Shape
{
public:
    Object() = default;
    virtual ~Object() = default;
    virtual void getIsecData(IntersectionData &isec) const = 0;
};

class SimpleObject: public Object
{
public:
    SimpleObject(const std::shared_ptr<Shape> &s, const std::shared_ptr<Material> &m):shape(s), material(m)
    {
        if(!this->shape) this->shape = primitives::Invisible;
        if(!this->material) this->material = material::DiffuseWhite;
    }
    virtual ~SimpleObject() = default;

    virtual bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        if (shape->intersection(ray, isec))
        {
            isec.object = this;
            return true;
        }
        return false;
    }
    virtual bool intersection(const Ray &ray) const override
    {
        return shape->intersection(ray);
    }
    virtual AABB getAABB() const override
    {
        return shape->getAABB();
    }
    virtual void getIsecData(IntersectionData &isec) const override
    {
        isec.material = material.get();
        if(isec.material->texture)
        {
            isec.primitive->getNormalAndUV(isec); // get normal and uv coord
            isec.albedo = isec.material->Kd * isec.material->texture->get(isec.uv);
        }
        else
        {
            isec.primitive->getNormal(isec);
            isec.albedo = isec.material->Kd;
        }
    }

protected:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
};

class TransformedSimpleObject: public SimpleObject
{
public:
    TransformedSimpleObject(const std::shared_ptr<Shape> &s, const std::shared_ptr<Material> &m, const Matrix4 &t):
        SimpleObject(s, m), objectToWorld(t)
    {
        worldToObject = t.getInverse();
        objectToWorldNormal = worldToObject.getTranspose();
    }
    ~TransformedSimpleObject() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        return SimpleObject::intersection(worldToObject * ray, isec);
    }
    bool intersection(const Ray &ray) const override
    {
        return SimpleObject::intersection(worldToObject * ray);
    }
    void getIsecData(IntersectionData &isec) const override
    {
        Vector3 phit(isec.phit);
        isec.phit = worldToObject * phit;
        SimpleObject::getIsecData(isec);
        isec.normal = (objectToWorldNormal * isec.normal).normalize();
        isec.phit = phit;
    }
    AABB getAABB() const override
    {
        return objectToWorld * SimpleObject::getAABB();
    }
protected:
    Matrix4 objectToWorld;
    Matrix4 worldToObject;
    Matrix4 objectToWorldNormal;
};
