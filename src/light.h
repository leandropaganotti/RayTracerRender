#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"
#include <vector>
#include <float.h>
#include <memory>

class Light
{
public:     
    virtual Vector3f position  () const = 0;
    virtual float    distance  (const Vector3f &point) const = 0;
    virtual Vector3f direction (const Vector3f &point) const = 0;
    virtual Vector3f intensity (const Vector3f &point) const = 0;
    virtual ~Light() = default;
};

typedef std::vector<std::unique_ptr<Light>> LightVector;

class PointLight: public Light
{
    Vector3f pos;
    Vector3f color;
    float strength;
    float k;
public:
    PointLight(const Vector3f &pos={0.0f}, const Vector3f &color={1.0f}, float strength=1.0f, float k=0.01f):
        pos(pos), color(color), strength(strength), k(k)
    {}
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
        return (pos - point).normalize();
    }
    Vector3f intensity(const Vector3f &point) const
    {
        return strength * color * attenuation(point);
    }
    float attenuation(const Vector3f &point) const
    {
        float d = distance(point);
//      return 1.0f / ( 1.0f + k * d * d);
        return 1.0f / (4 * M_PI * d*d);
    }

    Vector3f getPos() const;
    void     setPos(const Vector3f &value);
    Vector3f getColor() const;
    void     setColor(const Vector3f &value);
    float    getStrength() const;
    void     setStrength(float value);
    float    getK() const;
    void     setK(float value);
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
        this->dir.normalize();
    }
    Vector3f position() const
    {
        return FLT_MAX;
    }
    float distance(const Vector3f &) const
    {
        return FLT_MAX;
    }
    Vector3f direction(const Vector3f &) const
    {
        return -dir;
    }
    Vector3f intensity(const Vector3f &) const
    {
        return strength * color;
    }
    Vector3f getDir() const;
    void     setDir(const Vector3f &value);
    Vector3f getColor() const;
    void     setColor(const Vector3f &value);
    float    getStrength() const;
    void     setStrength(float value);
};

#endif // LIGHT_H
