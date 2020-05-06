#pragma once

#include <memory>
#include "vector.h"
#include "intersection.h"
#include "matrix.h"
#include "ray.h"
#include "material.h"

class Shape
{
public:
    virtual bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const = 0;
    virtual bool intersection(const Ray &ray, float tmax) const = 0;
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const = 0;
    virtual Vector2 uv(const Vector3 &phit, size_t idx) const = 0;
    virtual const Material* material(const Vector3 &phit, size_t idx) const = 0;
    virtual void fetch(const Ray &ray, IntersectionData &isec) const = 0;

    virtual void setTransformation(const Matrix4 &);

    Shape() = default;
    virtual ~Shape() = default;
};

class ShapeWithMaterial: public Shape
{
public:
    const Material* material(const Vector3 &, size_t) const override;

    void setMaterial(const std::shared_ptr<Material> &value);
    std::shared_ptr<Material> getMaterial() const;
protected:
    ShapeWithMaterial();
    virtual ~ShapeWithMaterial() override;
    std::shared_ptr<Material> mat;
};

class Instance: public ShapeWithMaterial
{
public:
    virtual ~Instance() override = default;
    virtual bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    virtual bool intersection(const Ray &ray, float tmax) const override;
    virtual Vector3 normal(const Vector3 &phit, size_t idx) const override;
    virtual Vector2 uv(const Vector3 &phit, size_t idx) const override;
    const Material *material(const Vector3 &phit, size_t idx) const override;
    void fetch(const Ray &ray, IntersectionData &isec) const override;

    virtual void setTransformation(const Matrix4 &transformation) override;

protected:
    Instance(std::shared_ptr<Shape> shape);

    std::shared_ptr<Shape> shape;

    Matrix4 model;              // object-to-world
    Matrix4 inverse;            // world-to-object
    Matrix4 inverseTranspose;   // matrix for normals transformation

    friend class Shapes;    
};
