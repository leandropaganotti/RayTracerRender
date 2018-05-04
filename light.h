#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"
#include <vector>
#include <float.h>

class Light
{
public:     
    virtual Vector3f  position() const = 0;
    virtual float     distance(const Vector3f &point) const = 0;
    virtual Vector3f direction(const Vector3f &point) const = 0;
    virtual Vector3f intensity(const Vector3f &point) const = 0;
};


class PointLight: public Light
{
    Vector3f pos;
    Vector3f color;
    float strength;
    float k;
public:
    PointLight(const Vector3f &pos={0.0f}, const Vector3f &color={1.0f}, float strength=1.0f, float k=0.05f):
        pos(pos), color(color), strength(strength), k(k)
    {

    }
    Vector3f position() const
    {
        return pos;
    }
    float distance(const Vector3f &point) const
    {
        return (point - pos).length();
    }
    Vector3f direction(const Vector3f &point) const
    {
        return (point - pos).normalize();
    }
    Vector3f intensity(const Vector3f &point) const
    {
        return strength * color * attenuation(point);
    }

    float attenuation(const Vector3f &point) const
    {
        //attenuation = 1.0f / ( 1.0f + scene.lights[i].attenuation * distToLight * distToLight);
        float d = distance(point);
        return 1.0f / ( 1.0f + k * d * d);
    }
};


class DistantLight: public Light
{
    Vector3f dir;
    Vector3f color;
    float strength;
public:
    DistantLight(const Vector3f &dir={0.0f, -1.0f, 0.0f}, const Vector3f &color={1.0f}, float strength=1.0f):
        dir(dir), color(color), strength(strength)
    {

    }
    Vector3f position() const
    {
        return FLT_MAX;
    }
    float distance(const Vector3f &point) const
    {
        return FLT_MAX;
    }
    Vector3f direction(const Vector3f &point) const
    {
        return dir;
    }
    Vector3f intensity(const Vector3f &point) const
    {
        return strength * color;
    }
};

typedef std::vector<Light*> LightVector;

#endif // LIGHT_H
