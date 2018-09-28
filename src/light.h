#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"
#include <vector>
#include <float.h>
#include <memory>

class Light
{
public:     
    virtual Vector3 position  () const = 0;
    virtual float    distance  (const Vector3 &point) const = 0;
    virtual Vector3 direction (const Vector3 &point) const = 0;
    virtual Vector3 intensity (const Vector3 &point) const = 0;
    virtual ~Light() = default;
};

typedef std::vector<std::unique_ptr<Light>> LightVector;

class PointLight: public Light
{
    Vector3 pos;
    Vector3 color;
    float strength;
    float k;
public:
    PointLight(const Vector3 &pos={0.0f}, const Vector3 &color={1.0f}, float strength=1.0f, float k=0.01f):
        pos(pos), color(color), strength(strength), k(k)
    {}
    Vector3 position() const
    {
        return pos;
    }
    float distance(const Vector3 &point) const
    {
        return (point - pos).length();
    }
    Vector3 direction(const Vector3 &point) const
    {
        return (pos - point).normalize();
    }
    Vector3 intensity(const Vector3 &point) const
    {
        return strength * color * attenuation(point);
    }
    float attenuation(const Vector3 &point) const
    {
        float d = distance(point);
        return 1.0f / ( 1.0f + k * d * d);
    }

    Vector3 getPos() const;
    void     setPos(const Vector3 &value);
    Vector3 getColor() const;
    void     setColor(const Vector3 &value);
    float    getStrength() const;
    void     setStrength(float value);
    float    getK() const;
    void     setK(float value);
};

class DistantLight: public Light
{
    Vector3 dir;
    Vector3 color;
    float strength;
public:
    DistantLight(const Vector3 &dir={0.0f, -1.0f, 0.0f}, const Vector3 &color={1.0f}, float strength=1.0f):
        dir(dir), color(color), strength(strength)
    {
        this->dir.normalize();
    }
    Vector3 position() const
    {
        return FLT_MAX;
    }
    float distance(const Vector3 &) const
    {
        return FLT_MAX;
    }
    Vector3 direction(const Vector3 &) const
    {
        return -dir;
    }
    Vector3 intensity(const Vector3 &) const
    {
        return strength * color;
    }
    Vector3 getDir() const;
    void     setDir(const Vector3 &value);
    Vector3 getColor() const;
    void     setColor(const Vector3 &value);
    float    getStrength() const;
    void     setStrength(float value);
};

#endif // LIGHT_H
