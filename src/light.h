#pragma once

#include "vector.h"
#include <vector>
#include <float.h>
#include <memory>

struct LightData
{
    Vector3 intensity;
    Vector3 direction;
    float   distance;
};

class LightIF
{
public:
    virtual void getLightData(const Vector3 &phit, LightData &light) const = 0;
    virtual ~LightIF(){}
};

class PointLight: public LightIF
{
public:
    PointLight(const Vector3 &position={0.0f}, const Vector3 &color={1.0f}, float strength=1.0f, float k=0.01f);

    float attenuation(const Vector3 &point) const;
    void getLightData(const Vector3 &phit, LightData &light) const override;

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

class DistantLight: public LightIF
{
public:
    DistantLight(const Vector3 &direction={0.0f, -1.0f, 0.0f}, const Vector3 &color={1.0f}, float strength=1.0f);
    void getLightData(const Vector3 &phit, LightData &light) const;

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

