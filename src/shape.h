#pragma once
#include <memory>
#include "intersection.h"
#include "transformation.h"
#include <iostream>

class Shape: public IntersectionIF, public TransformationIF
{
public:    
	virtual const Vector3 normal(const Vector3 &phit, size_t idx) const = 0;
	virtual const std::pair<float, float> uv(const Vector3 &phit, size_t idx) const = 0;

    // Pre defined Shapes
    static const std::shared_ptr<Shape> Invisible;       

protected:
	Shape() = default;
	virtual ~Shape() = default;    
};

class Instance: public Shape
{
public:
    static std::shared_ptr<Instance> Create(std::shared_ptr<Shape> shape=nullptr)
    {
        return std::shared_ptr<Instance>(new Instance(shape));
    }

    void setShape(std::shared_ptr<Shape> shape)
    {
        this->shape = shape ? shape : this->shape;
    }

    // IntersectionIF interface
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const override
    {
        Ray r = inverse * ray;
        if (shape->intersection(r, isec))
        {
            Vector3 phit = r.origin + isec.tnear * r.direction;
            phit = model * phit;
            isec.tnear = (phit - ray.origin).length();
            return true;
        }
        else
            return false;
    }
    virtual bool intersection(const Ray &ray, float &tnear) const override
    {
        Ray r = inverse * ray;
        if (shape->intersection(r, tnear))
        {
            Vector3 phit = r.origin + tnear * r.direction;
            phit = model * phit;
            tnear = (phit - ray.origin).length();
            return true;
        }
        else
            return false;
    }

    // Shape interface
public:
    virtual const Vector3 normal(const Vector3 &phit, size_t idx) const override
    {
        return (inverseTranspose * shape->normal(inverse * phit, idx)).normalize();
    }
    virtual const std::pair<float, float> uv(const Vector3 &phit, size_t idx) const override
    {
        return shape->uv(inverse * phit, idx);
    }

protected:
    Instance(std::shared_ptr<Shape> shape)
    {
        this->shape = shape ? shape : Shape::Invisible;
    }
    std::shared_ptr<Shape> shape;
};

class InvisibleShape: public Shape
{
public:
    static std::shared_ptr<InvisibleShape> Create()
    {
        return std::shared_ptr<InvisibleShape>(new InvisibleShape());
    }
    // IntersectionIF interface
public:
    bool intersection(const Ray &, IntersectionData &) const override;
    bool intersection(const Ray &, float &) const override;

    // Shape interface
public:
    const Vector3 normal(const Vector3 &, size_t) const override;
    const std::pair<float, float> uv(const Vector3 &, size_t) const override;

private:
    InvisibleShape() = default;
};
