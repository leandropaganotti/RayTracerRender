#include "light.h"

Vector3f PointLight::getPos() const
{
    return pos;
}

void PointLight::setPos(const Vector3f &value)
{
    pos = value;
}

Vector3f PointLight::getColor() const
{
    return color;
}

void PointLight::setColor(const Vector3f &value)
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


Vector3f DistantLight::getColor() const
{
    return color;
}

void DistantLight::setColor(const Vector3f &value)
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

Vector3f DistantLight::getDir() const
{
    return dir;
}

void DistantLight::setDir(const Vector3f &value)
{
    dir = value;
}
