#pragma once

#include "primitive.h"
#include "vector.h"

class Sphere: public Primitive
{    
public:
    Sphere(const Vector3 &center={0.0f}, const float &radius=0.5f);
    ~Sphere() = default;

    bool intersection(const Ray &ray, IntersectionData &isec) const override;
    bool intersection(const Ray& ray) const override;
    void getNormalAndUV(IntersectionData &isec) const override;
    void getNormal(IntersectionData& isec) const override;
    void getUV(IntersectionData &isec) const override;
    AABB getAABB() const override;
    void getSample(const Vector3 &from, Vector3 &dir, float &t, float &_1_pdf) const override;;

    Vector3  getCenter() const;
    void     setCenter(const Vector3 &value);
    float    getRadius() const;
    void     setRadius(float value);    

protected:
    Vector3 center;                 // position of the sphere
    float radius;                   // sphere radius and radius^2
    float radius2;
};
