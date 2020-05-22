#pragma once

#include "shape.h"
#include "matrix.h"
#include "aabb.h"
#include <memory>

class Instance: public Shape
{
public:
    Instance(std::shared_ptr<Shape> shape);
    virtual ~Instance() override;

    bool intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool intersection(const Ray &ray, float tmax) const override;
    Vector3 getNormal(const Vector3 &phit, size_t idx) const override;
    Vector2 getUV(const Vector3 &phit, size_t idx) const override;
    virtual void getIsecData(const Ray &, IntersectionData &isec) const override;
    AABB getAABB() const override;
    const Material * getMaterial(size_t idx) const override;

    void setTransformation(const Matrix4 &transformation);

protected:
    std::shared_ptr<Shape> shape;

    Matrix4 model;              // object-to-world
    Matrix4 inverse;            // world-to-object
    Matrix4 inverseTranspose;   // matrix for normals transformation
    AABB aabb;

    void updateAABB();
};
