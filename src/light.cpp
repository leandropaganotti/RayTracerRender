#include "light.h"
#include "utils.h"
#include "material.h"
#include "scene.h"
#include "objectfactory.h"

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
        light->setPosition(params.get<Vector3>("position"));
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

void PointLight::getSample(const Vector3 &phit, SampleLi &light) const
{
    light.direction = position - phit;
    light.distance = light.direction.length2();
    light.Li = intensity * (1.0f / ( 1.0f + atten * light.distance )); // attenuation = 1/(1+atten*dist*dist) -> default atten=4*pi
    light._1_pdf = 1.0f;
    light.distance = sqrt(light.distance);
    light.direction /= light.distance;
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
    return scene->intersection(ray) ? 0.0f : 1.0f;
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

void DistantLight::getSample(const Vector3 &, SampleLi &light) const
{
    light.direction = -direction;
    light.Li = intensity;
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
    ray.tmax = (ray.origin - sphere->getCenter()).length() - sphere->getRadius();
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
    if(scene->intersection(ray)) return 0.0f;
    return 1.0f;
}

void SphericalLight::setPosition(const Vector3 &p)
{
    sphere->setCenter(p);
}

void SphericalLight::setRadius(float radius)
{
    sphere->setRadius(radius);
}

float SphericalLight::getAttenuation() const
{
    return atten;
}

void SphericalLight::setAttenuation(float value)
{
    atten = value;
}

SphericalLight::SphericalLight():
    SimpleObject(std::make_shared<Sphere>(), Material::Create())
{
    sphere = dynamic_cast<Sphere*>(shape.get());
    material->E = intensity;
    material->type = MaterialType::LIGHT;
    atten = 4 * M_PI;
}

void SphericalLight::getSample(const Vector3 &phit, SampleLi &sampleLi) const
{
    sphere->getSample(phit, sampleLi.direction,  sampleLi._1_pdf);
    sampleLi.distance = (phit - sphere->getCenter()).length() - sphere->getRadius() - 0.001f;
    sampleLi.Li = intensity / ( 1 + atten * sampleLi.distance*sampleLi.distance);

}

float SphericalLightShadowOff::visibility(const Ray &, const std::vector<std::shared_ptr<Object> > &) const
{
    return 1.0f;
}

float SphericalLightShadowOff::visibility(const Ray &, const Scene *) const
{
    return 1.0f;
}
