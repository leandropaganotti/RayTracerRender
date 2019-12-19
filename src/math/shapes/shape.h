#pragma once

#include <memory>
#include "intersection.h"
#include "transformation.h"

class Shape: public IntersectionIF, public TransformationIF
{
public:
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const = 0;
    virtual std::pair<float, float> uv(const Vector3 &phit, size_t idx) const = 0;

protected:
	Shape() = default;
	virtual ~Shape() = default;
};

class Instance: public Shape {    
    // TransformationIF interface
public:
    virtual void setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale) override;

    // IntersectionIF interface
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const override;
    virtual bool intersection(const Ray &ray, float &tnear) const override;

    // Shape interface
public:
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const override;
    virtual std::pair<float, float> uv(const Vector3 &phit, size_t idx) const override;

protected:
    Instance(std::shared_ptr<Shape> shape=nullptr);
    virtual ~Instance() = default;

    std::shared_ptr<Shape> shape;
};

class LocalInstance: public Instance
{
public:
    virtual ~LocalInstance() = default;

    // IntersectionIF interface
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const override;
    virtual bool intersection(const Ray &ray, float &tnear) const override;

    // Shape interface
public:
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const override;
    virtual std::pair<float, float> uv(const Vector3 &phit, size_t idx) const override;

    // TransformationIF interface
public:
    virtual void setTransformation(const Vector3 &translate, const Vector3 &rotate, const Vector3 &scale) override;

protected:
    LocalInstance(std::shared_ptr<Shape> shape=nullptr);

    friend class Shapes;

protected:
    Matrix4 model;              // object-to-world
    Matrix4 inverse;            // world-to-object
    Matrix4 inverseTranspose;   // matrix for normals transformation
};