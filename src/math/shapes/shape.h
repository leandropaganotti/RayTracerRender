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

class Instance: public Shape
{
public:
    virtual ~Instance() = default;
    void setShape(std::shared_ptr<Shape> shape);

    // IntersectionIF interface
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const override;
    virtual bool intersection(const Ray &ray, float &tnear) const override;

    // Shape interface
public:
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const override;
    virtual std::pair<float, float> uv(const Vector3 &phit, size_t idx) const override;

protected:
    Instance(std::shared_ptr<Shape> shape);

    std::shared_ptr<Shape> shape;

    friend class Shapes;
};
