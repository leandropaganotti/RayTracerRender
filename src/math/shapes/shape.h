#pragma once

#include <memory>
#include "intersection.h"
#include "transformation.h"

class Shape: public IntersectionIF, public TransformationIF
{
public:
	virtual const Vector3 normal(const Vector3 &phit, size_t idx) const = 0;
	virtual const std::pair<float, float> uv(const Vector3 &phit, size_t idx) const = 0;

protected:
	Shape() = default;
	virtual ~Shape() = default;
};

class InstancedShape: public Shape
{
    // IntersectionIF interface
public:
    virtual bool intersection(const Ray &ray, IntersectionData &isec) const override;
    virtual bool intersection(const Ray &ray, float &tnear) const override;

    // Shape interface
public:
    virtual const Vector3 normal(const Vector3 &phit, size_t idx) const override;
    virtual const std::pair<float, float> uv(const Vector3 &phit, size_t idx) const override;

protected:
    InstancedShape(std::shared_ptr<Shape> shape);
    virtual ~InstancedShape() = default;
    std::shared_ptr<Shape> shape;
};

class Instance: public InstancedShape
{
public:
    void setShape(std::shared_ptr<Shape> shape);
private:
    Instance(std::shared_ptr<Shape> shape);
    friend class Shapes;
};
