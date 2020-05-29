#include "light.h"
#include "utils.h"

std::unique_ptr<Light> Light::Create(LightType type, bool shadow)
{
    switch (type) {
    case LightType::PointLight:
        if(shadow)
            return std::unique_ptr<PointLight>(new PointLight);
        else
            return std::unique_ptr<PointLightShadowOff>(new PointLightShadowOff);

    case LightType::DistantLight:
        if(shadow)
            return std::unique_ptr<DistantLight>(new DistantLight);
        else
            return std::unique_ptr<DistantLightShadowOff>(new DistantLightShadowOff);
    }
    return nullptr;
}

PointLight::PointLight(const Vector3 &pos, const Vector3 &color, float strength, float k):
    position(pos), color(color), strength(strength), k(k)
{
}

float PointLight::attenuation(const Vector3 &point) const
{
    float d = point.distance(point);
    return 1.0f / ( 1.0f + k * d * d);
}

void PointLight::getLightData(const Vector3 &phit, LightData &light) const
{
    light.direction = phit - position;
    light.distance = light.direction.length2();
    light.intensity = strength * color / ( 4 * M_PI * light.distance);
    light.distance = sqrtf(light.distance);
    light.direction /= light.distance;
}

float PointLight::visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects)
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

Vector3 PointLight::getColor() const
{
    return color;
}

void PointLight::setColor(const Vector3 &value)
{
    color = value;
}

float PointLight::getStrength() const
{
    return strength;
}

void PointLight::setStrength(float value)
{
    strength = value;
}

float PointLight::getK() const
{
    return k;
}

void PointLight::setK(float value)
{
    k = value;
}

float PointLightShadowOff::visibility(const Vector3 &, const std::vector<std::shared_ptr<Shape> > &)
{
    return 1.0f;
}

Vector3 DistantLight::getColor() const
{
    return color;
}

void DistantLight::setColor(const Vector3 &value)
{
    color = value;
}

float DistantLight::getStrength() const
{
    return strength;
}

void DistantLight::setStrength(float value)
{
    strength = value;
}

DistantLight::DistantLight(const Vector3 &dir, const Vector3 &color, float strength):
    direction(dir), color(color), strength(strength)
{
    this->direction.normalize();
}

void DistantLight::getLightData(const Vector3 &, LightData &light) const
{
    light.distance = INFINITY;
    light.direction = direction;
    light.intensity = color * strength;
}

float DistantLight::visibility(const Vector3 &phit, const std::vector<std::shared_ptr<Shape> > &objects)
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

Vector3 DistantLight::getDirection() const
{
    return direction;
}

void DistantLight::setDirection(const Vector3 &value)
{
    direction = value;
    direction.normalize();
}

float Light::visibility(const Vector3 &, const std::vector<std::shared_ptr<Shape> > &)
{
    return 1.0f;
}

float DistantLightShadowOff::visibility(const Vector3 &, const std::vector<std::shared_ptr<Shape> > &)
{
    return 1.0f;
}
