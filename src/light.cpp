#include "light.h"
#include "utils.h"
#include "material.h"
#include "scene.h"
#include "intersectiondata.h"

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
    case LightType::AreaLight:
    {
        std::shared_ptr<AreaLight> light(new AreaLight(params.get<std::shared_ptr<SimpleObject>>("object")));
        return light;
    }
    }
    return nullptr;
}

PointLight::PointLight()
{
    atten = 4*M_PI;
}

void PointLight::getLightData(const Vector3 &phit, LightData &light) const
{
    light.direction = position - phit;
    light.distance = light.direction.length2();
    light.Li = emission * color * (1.0f / ( 1.0f + atten * light.distance )); // attenuation = 1/(1+atten*dist*dist) -> default atten=4*pi
    light.distance = sqrtf(light.distance);
    light.direction /= light.distance;
    light.pdf = 1.0f;
}

float PointLight::visibility(const Ray &ray, const Scene *scene) const
{
    ray.tmax = (ray.origin - position).length();
    if(scene->intersection(ray)) return 0.0f;
    return 1.0f;
}

const Vector3 &PointLight::getPosition() const
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

const Vector3 &PointLight::getColor() const
{
    return color;
}

void PointLight::setColor(const Vector3 &newColor)
{
    color = newColor;
}

const Vector3 &PointLight::getIntensity() const
{
    return emission;
}

void PointLight::setIntensity(const Vector3 &newIntensity)
{
    emission = newIntensity;
}

float PointLightShadowOff::visibility(const Ray &, const Scene *) const
{
    return 1.0f;
}

DistantLight::DistantLight()
{
    direction = Vector3(0, -1, 0);
}



void DistantLight::getLightData(const Vector3 &, LightData &light) const
{
    light.distance = INFINITY;
    light.direction = -direction;
    light.Li = emission * color;
    light.pdf = 1.0f;
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

const Vector3 &DistantLight::getColor() const
{
    return color;
}

void DistantLight::setColor(const Vector3 &newColor)
{
    color = newColor;
}

const Vector3 &DistantLight::getIntensity() const
{
    return emission;
}

void DistantLight::setIntensity(const Vector3 &newIntensity)
{
    emission = newIntensity;
}

float DistantLightShadowOff::visibility(const Ray &, const Scene *) const
{
    return 1.0f;
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

const Vector3 &SphericalLight::getColor() const
{
    return color;
}

void SphericalLight::setColor(const Vector3 &newColor)
{
    color = newColor;
}

const Vector3 &SphericalLight::getIntensity() const
{
    return emission;
}

void SphericalLight::setIntensity(const Vector3 &newIntensity)
{
    emission = newIntensity;
}

SphericalLight::SphericalLight()
{
    atten = 4 * M_PI;
}

void SphericalLight::getLightData(const Vector3 &phit, LightData &light) const
{
    //sampleSolidAngleSphere(phit, light.direction,  light._1_pdf);
    light.distance = (phit-center).length() - radius;
    light.direction = -light.direction;
    light.Li = emission * 1/(1+atten*light.distance*light.distance);
}

float SphericalLightShadowOff::visibility(const Ray &, const Scene *) const
{
    return 1.0f;
}

AreaLight::AreaLight(const std::shared_ptr<SimpleObject> &object): object(object)
{
    atten = 4 * M_PI;
}

void AreaLight::getLightData(const Vector3 &phit, LightData &light) const
{
    object->getShape()->getSample(phit, light.direction, light.distance, light.pdf);
    light.Li = object->getMaterial()->emission * object->getMaterial()->Kd;// * 1.0f/(light.distance*light.distance);
}

float AreaLight::visibility(const Ray &ray, const Scene *scene) const
{
    if(scene->intersection(ray)) return 0.0f;
    return 1.0f;
}

void AreaLight::getSample(const IntersectionData &isec, Vector3 &dir, float &_1_pdf) const
{
}

const std::shared_ptr<SimpleObject> &AreaLight::getObject() const
{
    return object;
}

void AreaLight::setObject(const std::shared_ptr<SimpleObject> &newObject)
{
    object = newObject;
}

bool VisibilityTester::visibility(const Scene *scene)
{
    Vector3 d = p2 - p1;
    float dist = d.length();
    d /= dist;
    Ray ray(p1 + 0.0001 * d, d, dist - 0.0001);
    return scene->intersection(ray);
}
