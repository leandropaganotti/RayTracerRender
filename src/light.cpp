#include "light.h"

Vector3 PointLight::getPos() const
{
    return pos;
}

void PointLight::setPos(const Vector3 &value)
{
    pos = value;
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

Vector3 DistantLight::getDir() const
{
    return dir;
}

void DistantLight::setDir(const Vector3 &value)
{
    dir = value;
}
