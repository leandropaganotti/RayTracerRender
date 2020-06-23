#pragma once

#include "shape.h"
#include "instance.h"

class Sphere: public Shape
{    
public:
    Sphere(const Vector3 &center={0.0f}, const float &radius=1.0f);
    virtual ~Sphere();

    bool  intersection(const Ray &ray, IntersectionData &isec) const override;
    bool  intersection(const Ray& ray) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData &isec) const override;
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

    void getIsecData(const Ray &ray, IntersectionData &isec) const override;

    void setMaterial(const std::shared_ptr<Material> &value);

    const Material* getMaterial(size_t) const override;

protected:
    std::shared_ptr<Material> material;
};

class GEllipsoid: public Instance
{
public:
    GEllipsoid();

    void setMaterial(const std::shared_ptr<Material> &value);

    const Material* getMaterial(size_t) const override;

protected:
    std::shared_ptr<Material> material;
};
