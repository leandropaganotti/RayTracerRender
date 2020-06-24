#pragma once

#include "matrix.h"
#include "shape.h"
#include "material.h"
#include <memory>
#include "invisible.h"

class Object: public IntersectionIF
{
public:
    virtual ~Object(){}
    virtual void getIsecData(IntersectionData &isec) const = 0;
};

class SimpleObject: public Object
{
public:
    SimpleObject(const std::shared_ptr<Shape> &s, const std::shared_ptr<Material> &material):shape(s), material(material)
    {
        if(!shape) shape = InvisibleShape::GetInstance();
        if(!this->material) this->material = material::DiffuseWhite;
    }
    virtual ~SimpleObject(){}

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        if (shape->intersection(ray, isec))
        {
            isec.material = material.get();
            isec.object = this;
            return true;
        }
        return false;
    }
    bool intersection(const Ray &ray) const override
    {
        return shape->intersection(ray);
    }
    AABB getAABB() const override
    {
        return shape->getAABB();
    }
    void getIsecData(IntersectionData &isec) const override
    {
        if(material->texture)
        {
            shape->getIsecData(isec); // get normal and uv coord
            isec.albedo = material->Kd * material->texture->get(isec.uv);
        }
        else
        {
            shape->getNormal(isec);
            isec.albedo = material->Kd;
        }
    }
    const Material* getMaterial() const
    {
        return material.get();
    }
    void setMaterial(const std::shared_ptr<Material> &value)
    {
        material = value ? value : material::DiffuseWhite;
    }

protected:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
};

class TransformedObject: public Object
{
public:
    TransformedObject(const std::shared_ptr<Object> &o, const Matrix4 &t):
        object(o), model(t)
    {
        if(!object) object = std::shared_ptr<SimpleObject>(new SimpleObject(nullptr, nullptr));
        inverse = t.getInverse();
        inverseTranspose = inverse.getTranspose();
    }
    TransformedObject(const std::shared_ptr<Shape> &shape, const std::shared_ptr<Material> &material, const Matrix4 &transform):
        object(std::shared_ptr<SimpleObject>(new SimpleObject(shape, material))), model(transform)
    {
        inverse = transform.getInverse();
        inverseTranspose = inverse.getTranspose();
    }

    bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        Ray ray_local = inverse * ray;
        if (object->intersection(ray_local, isec))
        {
            isec.object = this;
            return true;
        }
        return false;
    }
    bool intersection(const Ray &ray) const override
    {
        Ray ray_local = inverse * ray;
        return object->intersection(ray_local);
    }
    AABB getAABB() const override
    {
        return model * object->getAABB();
    }
    void getIsecData(IntersectionData &isec) const override
    {
        Vector3 phit(isec.phit);
        isec.phit = inverse * phit;
        object->getIsecData(isec);
        isec.normal = (inverseTranspose * isec.normal).normalize();
        isec.phit = phit;
    }
private:
    std::shared_ptr<Object> object;
    Matrix4 model;
    Matrix4 inverse;
    Matrix4 inverseTranspose;
};




