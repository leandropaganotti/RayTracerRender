#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"
#include "texture.h"

class Sphere: public Object
{
    Vector3f center;                        // position of the sphere
    float radius, radius2;                  // sphere radius and radius^2
    std::unique_ptr<Texture> tex;
public:    
    Sphere( const Vector3f &center={0.0f}, const float &radius=1.0f, const Vector3f &color={1.0f});

    bool  intersection(const Ray &ray, IntersectionData &isec) const;
    bool  intersection(const Ray& ray, float& tnear) const;
    const Vector3f normal(const Vector3f &phit, size_t) const;
    const Vector3f texture(const Vector3f &phit, size_t) const;

    Vector3f getCenter() const;
    void     setCenter(const Vector3f &value);
    float    getRadius() const;
    void     setRadius(float value);

    std::unique_ptr<Texture>& getTex();
};

#endif  // SPHERE_H
