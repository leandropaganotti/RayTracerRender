#include "sphere.h"


Sphere::Sphere(const Vector3f &center, const float &radius, const Vector3f &color) :
    center(center), radius(radius), radius2(radius * radius)
{
    c_diffuse = color;
}

bool Sphere::intersection(const Ray &ray, IntersectionData &isec) const
{
    Vector3f l = center - ray.origin;
    float tca = l.dot(ray.direction);
    if (tca < 0) return false;
    float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;

    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    isec.tnear = t0 < 0 ? t1 : t0;
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = (isec.phit - center).normalize();
    isec.object = this;

    return true;
}

bool Sphere::intersection(const Ray &ray, float &tnear) const
{
    Vector3f l = center - ray.origin;
    float tca = l.dot(ray.direction);
    if (tca < 0) return false;
    float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;

    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    tnear = t0 < 0 ? t1 : t0;

    return true;
}
