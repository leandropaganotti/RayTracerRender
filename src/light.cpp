#include "light.h"
#include "utils.h"
#include "material.h"
#include "scene.h"
#include "intersectiondata.h"

Light::Light():intensity(1), color(1){}

Light::~Light(){}

std::shared_ptr<Light> Light::Create(LightType type, const ParamSet &params)
{
    switch (type) {
    case LightType::PointLight:
    {
        std::shared_ptr<PointLight> light;
        light = params.get<bool>("shadowoff") ? std::shared_ptr<PointLight>(new PointLightShadowOff) : std::shared_ptr<PointLight>(new PointLight);
        light->setPosition(params.get<Vector3>("position"));
        light->setColor(params.get<Vector3>("color"));
        light->setIntensity(params.get<Vector3>("intensity"));
        light->setAttenuation(params.get<float>("atten"));
        return light;
    }
    case LightType::DistantLight:
    {
        std::shared_ptr<DistantLight> light;
        light = params.get<bool>("shadowoff") ? std::shared_ptr<DistantLight>(new DistantLightShadowOff) : std::shared_ptr<DistantLight>(new DistantLight);
        light->setDirection(params.get<Vector3>("direction"));
        light->setColor(params.get<Vector3>("color"));
        light->setIntensity(params.get<Vector3>("intensity"));
        return light;
    }
    case LightType::SphericalLight:
    {
        std::shared_ptr<SphericalLight> light;
        light = params.get<bool>("shadowoff") ? std::shared_ptr<SphericalLight>(new SphericalLightShadowOff) : std::shared_ptr<SphericalLight>(new SphericalLight);
        light->setCenter(params.get<Vector3>("position"));
        light->setRadius(params.get<float>("radius"));
        light->setColor(params.get<Vector3>("color"));
        light->setIntensity(params.get<Vector3>("intensity"));
        light->setAttenuation(params.get<float>("atten"));
        return light;
    }
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
    light._1_pdf = 1.0f;
}

float PointLight::visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const
{
    float vis = 1.0f;
    IntersectionData isec;
    ray.tmax = (ray.origin - position).length();
    for(auto &object : objects)
    {
        if (object->intersection(ray, isec))
        {
            if (isec.material->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

float PointLight::visibility(const Ray &ray, const Scene *scene) const
{
    ray.tmax = (ray.origin - position).length();
    if(scene->intersection(ray)) return 0.0f;
    return 1.0f;
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

float PointLightShadowOff::visibility(const Ray &, const std::vector<std::shared_ptr<Object> > &) const
{
    return 1.0f;
}

float PointLightShadowOff::visibility(const Ray &, const Scene *) const
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
    light._1_pdf = 1.0f;
}

float DistantLight::visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const
{
    float vis = 1.0f;
    IntersectionData isec;
    for(auto &object : objects)
    {
        if (object->intersection(ray, isec))
        {
            if (isec.material->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

float DistantLight::visibility(const Ray &ray, const Scene *scene) const
{
    ray.tmax = INFINITY;
    if(scene->intersection(ray)) return 0.0f;
    return 1.0f;
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

float DistantLightShadowOff::visibility(const Ray &, const std::vector<std::shared_ptr<Object> > &) const
{
    return 1.0f;
}

float DistantLightShadowOff::visibility(const Ray &, const Scene *) const
{
    return 1.0f;
}

float SphericalLight::visibility(const Ray &ray, const std::vector<std::shared_ptr<Object> > &objects) const
{
    float vis = 1.0f;
    IntersectionData isec;
    ray.tmax = (ray.origin - center).length() - radius;
    for(auto &object : objects)
    {
        if (object->intersection(ray, isec))
        {
            if (isec.material->type == MaterialType::TRANSPARENT)
                vis *= 0.8f;
            else
                return 0.0f;
        }
    }
    return vis;
}

float SphericalLight::visibility(const Ray &ray, const Scene *scene) const
{
    ray.tmax = (ray.origin - center).length() - radius;
    if(scene->intersection(ray)) return 0.0f;
    return 1.0f;
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

float SphericalLightShadowOff::visibility(const Ray &, const std::vector<std::shared_ptr<Object> > &) const
{
    return 1.0f;
}

float SphericalLightShadowOff::visibility(const Ray &, const Scene *) const
{
    return 1.0f;
}
