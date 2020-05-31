#include "light.h"
#include "utils.h"
#include "material.h"

Light::Light()
{
    intensity = Vector3(1);
    color = Vector3(1);
}

std::shared_ptr<Light> Light::Create(LightType type, bool shadow)
{
    switch (type) {
    case LightType::PointLight:
        if(shadow)
            return std::shared_ptr<PointLight>(new PointLight);
        else
            return std::shared_ptr<PointLightShadowOff>(new PointLightShadowOff);

    case LightType::DistantLight:
        if(shadow)
            return std::shared_ptr<DistantLight>(new DistantLight);
        else
            return std::shared_ptr<DistantLightShadowOff>(new DistantLightShadowOff);

    case LightType::SphericalLight:
        if(shadow)
            return std::shared_ptr<SphericalLight>(new SphericalLight);
        else
            return std::shared_ptr<SphericalLightShadowOff>(new SphericalLightShadowOff);
    }
    return nullptr;
}

Vector3 Light::getIntensity() const
{
    return intensity;
}

void Light::setIntensity(const Vector3 &value)
{
    intensity = value * color;
}

Vector3 Light::getColor() const
{
    return color;
}

void Light::setColor(const Vector3 &value)
{
    color = value;
    intensity = intensity * color;
}

Vector3 Light::forEachSample(const Vector3 &, const std::function<Vector3 (const LightData &)> &) const
{
    return Vector3(0);
}

PointLight::PointLight()
{
    atten = 4*M_PI;
}

PointLight::~PointLight()
{
}

void PointLight::getLightData(const Vector3 &phit, LightData &light) const
{
    light.direction = phit - position;
    light.distance = light.direction.length2();
    light.intensity = intensity * (1.0f / ( 1.0f + atten * light.distance )); // attenuation = 1/(1+atten*dist*dist) -> default atten=4*pi
    light.distance = sqrtf(light.distance);
    light.direction /= light.distance;
}

float PointLight::visibility(const Ray &ray, const std::vector<std::shared_ptr<Shape> > &objects) const
{
    float vis = 1.0f;
    IntersectionData isec;
    float tmax = (ray.origin - position).length();
    for(auto &object : objects)
    {
        if (object->intersection(ray, tmax, isec))
        {
            if (object->getMaterial(isec.idx)->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

float PointLight::visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects) const
{
    float vis = 1.0f;
    IntersectionData isec;
    Ray ray(phit, position-phit);
    float tmax = ray.direction.length();
    ray.direction /= tmax;
    for(auto &object : objects)
    {
        if (object->intersection(ray, tmax, isec))
        {
            if (object->getMaterial(isec.idx)->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

Vector3 PointLight::getPosition() const
{
    return position;
}

void PointLight::setPosition(const Vector3 &value)
{
    position = value;
}

float PointLight::getAttenuation() const
{
    return atten;
}

void PointLight::setAttenuation(float value)
{
    atten = value;
}

float PointLightShadowOff::visibility(const Vector3 &, const std::vector<std::shared_ptr<Shape> > &) const
{
    return 1.0f;
}

float PointLightShadowOff::visibility(const Ray &, const std::vector<std::shared_ptr<Shape> > &) const
{
    return 1.0f;
}

DistantLight::DistantLight()
{
    direction = Vector3(0, -1, 0);
}

DistantLight::~DistantLight()
{

}

void DistantLight::getLightData(const Vector3 &, LightData &light) const
{
    light.distance = INFINITY;
    light.direction = direction;
    light.intensity = intensity;
}

float DistantLight::visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects) const
{
    float vis = 1.0f;
    IntersectionData isec;
    Ray ray(phit, -direction);
    for(auto &object : objects)
    {
        if (object->intersection(ray, INFINITY, isec))
        {
            if (object->getMaterial(isec.idx)->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

float DistantLight::visibility(const Ray &ray, const std::vector<std::shared_ptr<Shape> > &objects) const
{
    float vis = 1.0f;
    IntersectionData isec;
    for(auto &object : objects)
    {
        if (object->intersection(ray, INFINITY, isec))
        {
            if (object->getMaterial(isec.idx)->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

Vector3 DistantLight::getDirection() const
{
    return direction;
}

void DistantLight::setDirection(const Vector3 &value)
{
    direction = value;
    direction.normalize();
}

float DistantLightShadowOff::visibility(const Vector3 &, const std::vector<std::shared_ptr<Shape> > &) const
{
    return 1.0f;
}

float DistantLightShadowOff::visibility(const Ray &, const std::vector<std::shared_ptr<Shape> > &) const
{
    return 1.0f;
}

float SphericalLight::visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects) const
{
    float vis = 1.0f, _1_pdf;
    Ray ray;
    ray.origin = phit;
    IntersectionData isec;
    sampleSolidAngleSphere(phit, ray.direction,  _1_pdf);
    float tmax = (ray.origin - center).length() - radius;
    for(auto &object : objects)
    {
        if (object->intersection(ray, tmax, isec))
        {
            if (object->getMaterial(isec.idx)->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

float SphericalLight::visibility(const Ray &ray, const std::vector<std::shared_ptr<Shape> > &objects) const
{
    float vis = 1.0f;
    IntersectionData isec;
    float tmax = (ray.origin - center).length() - radius;
    for(auto &object : objects)
    {
        if (object->intersection(ray, tmax, isec))
        {
            if (object->getMaterial(isec.idx)->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

Vector3 SphericalLight::forEachSample(const Vector3 &phit, const std::function<Vector3(const LightData &)> &fn) const
{
    Vector3 phitColor(0);
    size_t n=10;
    for(size_t i=0; i < n; ++i)
    {
        LightData data;
        sampleSolidAngleSphere(phit, data.direction,  data._1_pdf);
        data.distance = (phit-center).length() - radius;
        data.direction = -data.direction;
        data.intensity = intensity * 1/(1+atten*data.distance*data.distance);
        phitColor += fn(data);
    }

    return phitColor/n;
}

float SphericalLight::getAttenuation() const
{
    return atten;
}

void SphericalLight::setAttenuation(float value)
{
    atten = value;
}

SphericalLight::SphericalLight()
{
    atten = 4 * M_PI;
}

void SphericalLight::getLightData(const Vector3 &phit, LightData &light) const
{
    sampleSolidAngleSphere(phit, light.direction,  light._1_pdf);
    light.distance = (phit-center).length() - radius;
    light.direction = -light.direction;
    light.intensity = intensity * 1/(1+atten*light.distance*light.distance);

}

float SphericalLightShadowOff::visibility(const Vector3 &, const std::vector<std::shared_ptr<Shape> > &) const
{
    return 1.0f;
}

float SphericalLightShadowOff::visibility(const Ray &, const std::vector<std::shared_ptr<Shape> > &) const
{
    return 1.0f;
}
