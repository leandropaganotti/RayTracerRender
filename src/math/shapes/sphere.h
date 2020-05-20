#pragma once

#include "shape.h"
#include "instance.h"

class Sphere: public ShapeNormalUV
{    
public:
    Sphere(const Vector3 &center={0.0f}, const float &radius=1.0f);

    bool  intersection(const Ray &ray, float tmax, IntersectionData &isec) const override;
    bool  intersection(const Ray& ray, float tmax) const override;
    Vector3 getNormal(const Vector3 &phit, size_t) const override;
    Vector2 getUV(const Vector3 &phit, size_t) const override;
    virtual void fetchData(const Ray &ray, IntersectionData &isec) const override;
    AABB getAABB() const override;

    Vector3  getCenter() const;
    void     setCenter(const Vector3 &value);
    float    getRadius() const;
    void     setRadius(float value);    

    void sampleSolidAngleSphere(const Vector3& point, Vector3& sample,  float &_1_pdf) const;

protected:
    Vector3 center;                 // position of the sphere
    float radius;                   // sphere radius and radius^2
    float radius2;
};

class GSphere: public Sphere
{
public:
    GSphere(const Vector3 &c={0.0f}, const float &r=1.0f);

    void fetchData(const Ray &ray, IntersectionData &isec) const override;

    std::shared_ptr<Material> getMaterial() const;
    void setMaterial(const std::shared_ptr<Material> &value);

protected:
    std::shared_ptr<Material> material;
};

class GEllipsoid: public Instance
{
public:
    GEllipsoid();

    void fetchData(const Ray &ray, IntersectionData &isec) const override;

    std::shared_ptr<Material> getMaterial() const;
    void setMaterial(const std::shared_ptr<Material> &value);

protected:
    std::shared_ptr<Material> material;
};
