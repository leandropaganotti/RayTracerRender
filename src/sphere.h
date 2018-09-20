#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include "texture.h"
#include <math.h>
#include "utils.h"

class Sphere: public Object
{
    Vector3f center;                        // position of the sphere
    float radius, radius2;                  // sphere radius and radius^2    
public:    
    Sphere( const Vector3f &center={0.0f}, const float &radius=1.0f, const Vector3f &color={1.0f});

    bool  intersection(const Ray &ray, IntersectionData &isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;
    const Vector3f normal(const Vector3f &phit, size_t) const;

    Vector3f getCenter() const;
    void     setCenter(const Vector3f &value);
    float    getRadius() const;
    void     setRadius(float value);    

private:
    const std::pair<float, float> texUV(const Vector3f &phit, size_t) const;
};

inline
const std::pair<float, float> Sphere::texUV(const Vector3f &phit, size_t) const
{
    Vector3f d = (phit-center).normalize();
    float u = 0.5 + atan2f(d.z, d.x) / (2.0f * PI);
    float v = 0.5 - asinf(d.y) / PI;
    return std::make_pair(u, v);
}

#endif  // SPHERE_H
