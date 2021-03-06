#pragma once

#include "vector.h"
#include <vector>
#include <float.h>
#include <memory>
#include "sphere.h"
#include <functional>
#include "paramset.h"
#include "object.h"

class Scene;

enum class LightType
{
    PointLight,
    DistantLight,
    SphericalLight,
};

struct LightData
{
    Vector3 intensity;
    Vector3 direction;
    float   distance;
    float   _1_pdf;
};

class Light
{
public:
    Light();
    virtual ~Light();

    static std::shared_ptr<Light> Create(LightType type, const ParamSet &params);

    virtual void  getLightData(const Vector3 &phit, LightData &light) const = 0;
    virtual float visibility(const Ray &ray, const std::vector<std::shared_ptr<Object>> &objects) const = 0;
    virtual float visibility(const Ray &ray, const Scene *scene) const = 0;

    Vector3 getIntensity() const;
    void    setIntensity(const Vector3 &value);

    Vector3 getColor() const;
    void    setColor(const Vector3 &value);
protected:
    Vector3 intensity;
    Vector3 color;
};

class PointLight: public Light
{
public:
    PointLight();
    virtual ~PointLight();

    virtual void  getLightData(const Vector3 &phit, LightData &light) const override;
    virtual float visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const override;
    virtual float visibility(const Ray &ray, const Scene *scene) const override;

    Vector3  getPosition() const;
    void     setPosition(const Vector3 &value);

    float    getAttenuation() const;
    void     setAttenuation(float value);

protected:
    Vector3 position;
    float atten;
};

class PointLightShadowOff: public PointLight
{
public:
    float visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const override;
    float visibility(const Ray &ray, const Scene *scene) const override;
};

class DistantLight: public Light
{
public:
    DistantLight();
    virtual ~DistantLight();

    virtual void  getLightData(const Vector3 &phit, LightData &light) const override;
    virtual float visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const override;
    virtual float visibility(const Ray &ray, const Scene *scene) const override;

    Vector3  getDirection() const;
    void     setDirection(const Vector3 &value);

protected:
    Vector3 direction;
};

class DistantLightShadowOff: public DistantLight
{
public:
    float visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const override;
    float visibility(const Ray &ray, const Scene *scene) const override;
};

class SphericalLight: public Sphere, public Light
{
public:
    SphericalLight();
    virtual void  getLightData(const Vector3 &phit, LightData &light) const override;
    virtual float visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const override;
    virtual float visibility(const Ray &ray, const Scene *scene) const override;

    float getAttenuation() const;
    void  setAttenuation(float value);

protected:
    float atten;

};

class SphericalLightShadowOff: public SphericalLight
{
public:
    float visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const override;
    float visibility(const Ray &ray, const Scene *scene) const override;
};




