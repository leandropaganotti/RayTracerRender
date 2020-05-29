#pragma once

#include "vector.h"
#include <vector>
#include <float.h>
#include <memory>
#include "shape.h"
#include "material.h"

enum class LightType {
    PointLight,
    DistantLight
};

struct LightData
{
    Vector3 intensity;
    Vector3 direction;
    float   distance;
};

class Light
{
public:
    static std::unique_ptr<Light> Create(LightType type, bool shadow);

    virtual void  getLightData(const Vector3 &phit, LightData &light) const = 0;
    virtual float visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape>> &objects);
    virtual ~Light(){}
};

class PointLight: public Light
{
public:
    PointLight(const Vector3 &position={0.0f}, const Vector3 &color={1.0f}, float strength=1.0f, float k=0.01f);

    float attenuation(const Vector3 &point) const;
    void  getLightData(const Vector3 &phit, LightData &light) const override;
    virtual float visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects) override;

    Vector3  getPosition() const;
    void     setPosition(const Vector3 &value);
    Vector3  getColor() const;
    void     setColor(const Vector3 &value);
    float    getStrength() const;
    void     setStrength(float value);
    float    getK() const;
    void     setK(float value);

protected:
    Vector3 position;
    Vector3 color;
    float strength;
    float k;
};

class PointLightShadowOff: public PointLight
{
public:
    float visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects) override;
};

class DistantLight: public Light
{
public:
    DistantLight(const Vector3 &direction={0.0f, -1.0f, 0.0f}, const Vector3 &color={1.0f}, float strength=1.0f);
    void getLightData(const Vector3 &phit, LightData &light) const override;
    float visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects) override;

    Vector3  getDirection() const;
    void     setDirection(const Vector3 &value);
    Vector3  getColor() const;
    void     setColor(const Vector3 &value);
    float    getStrength() const;
    void     setStrength(float value);

protected:
    Vector3 direction;
    Vector3 color;
    float strength;
};

class DistantLightShadowOff: public DistantLight
{
public:
    float visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects) override;
};
