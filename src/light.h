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
    AreaLight
};

struct VisibilityTester
{
    bool visibility(const Scene *scene);
    Vector3 p1;
    Vector3 p2;
};

struct LightData
{
    Vector3 Li;
    Vector3 direction;
    float   distance;
    float   pdf;
};

class Light
{
public:
    Light() = default;
    virtual ~Light() = default;

    virtual void getSample(const IntersectionData &isec, Vector3 &dir, float &_1_pdf) const { _1_pdf = 0.0; };
    virtual void getSample(Vector3 &point, float &pdf) const { pdf = 0.0; };
    virtual void  getLightData(const Vector3 &phit, LightData &light) const = 0;
    virtual float visibility(const Ray &ray, const Scene *scene) const = 0;
    virtual Vector3 Le() const { return 0;}
    virtual Vector3 Li() const { return 0;};

    static std::shared_ptr<Light> Create(LightType type, const ParamSet &params);
};

class PointLight: public Light
{
public:
    PointLight();
    virtual ~PointLight() = default;

    virtual void  getLightData(const Vector3 &phit, LightData &light) const override;
    virtual float visibility(const Ray &ray, const Scene *scene) const override;

    const Vector3  &getPosition() const;
    void setPosition(const Vector3 &value);

    float getAttenuation() const;
    void setAttenuation(float value);

    const Vector3 &getColor() const;
    void setColor(const Vector3 &newColor);

    const Vector3 &getIntensity() const;
    void setIntensity(const Vector3 &newIntensity);

protected:
    Vector3 color;
    Vector3 emission;
    Vector3 position;
    float atten;
};

class PointLightShadowOff: public PointLight
{
public:
    float visibility(const Ray &ray, const Scene *scene) const override;
};

class DistantLight: public Light
{
public:
    DistantLight();
    virtual ~DistantLight() = default;

    virtual void  getLightData(const Vector3 &phit, LightData &light) const override;
    virtual float visibility(const Ray &ray, const Scene *scene) const override;

    Vector3  getDirection() const;
    void     setDirection(const Vector3 &value);

    const Vector3 &getColor() const;
    void setColor(const Vector3 &newColor);

    const Vector3 &getIntensity() const;
    void setIntensity(const Vector3 &newIntensity);

protected:
    Vector3 direction;
    Vector3 color;
    Vector3 emission;
};

class DistantLightShadowOff: public DistantLight
{
public:
    float visibility(const Ray &ray, const Scene *scene) const override;
};

class SphericalLight: public Light, public Sphere
{
public:
    SphericalLight();
    virtual ~SphericalLight() = default;
    virtual void  getLightData(const Vector3 &phit, LightData &light) const override;
    virtual float visibility(const Ray &ray, const Scene *scene) const override;

    float getAttenuation() const;
    void  setAttenuation(float value);

    const Vector3 &getColor() const;
    void setColor(const Vector3 &newColor);

    const Vector3 &getIntensity() const;
    void setIntensity(const Vector3 &newIntensity);

protected:
    float atten;
    Vector3 color;
    Vector3 emission;
};

class SphericalLightShadowOff: public SphericalLight
{
public:
    float visibility(const Ray &ray, const Scene *scene) const override;
};

class AreaLight: public Light
{
public:
    AreaLight(const std::shared_ptr<SimpleObject> &object);
    virtual ~AreaLight() = default;
    virtual void  getLightData(const Vector3 &phit, LightData &light) const override;
    virtual float visibility(const Ray &ray, const Scene *scene) const override;
    virtual void getSample(const IntersectionData &isec, Vector3 &dir, float &_1_pdf) const override;
    const std::shared_ptr<SimpleObject> &getObject() const;
    void setObject(const std::shared_ptr<SimpleObject> &newObject);

private:
    std::shared_ptr<SimpleObject> object;
    float atten;
};


