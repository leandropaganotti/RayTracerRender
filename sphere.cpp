#include "sphere.h"


Sphere::Sphere(const Vector3f &center, const float &radius, const Vector3f &color) :
    Object(color),
    center(center), radius(radius), radius2(radius * radius)
{
}

bool Sphere::intersection(const Ray &ray, IntersectionData &inter) const
{
    float t0, t1;
    Vector3f l = center - ray.origin;
    float tca = l.dot(ray.direction);
    if (tca < 0) return false;
    float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;
    float thc = sqrt(radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    if (t0 < 0) t0 = t1;

    Vector3f t0_ray = t0 * ray.direction;

    inter.phit = ray.origin + t0_ray;
    inter.dist = t0_ray.length();
    inter.normal = (inter.phit - center).normalize();
    inter.object = this;

    return true;
}

bool Sphere::intersection(const Ray &ray, float &dist) const
{
    Vector3f l = center - ray.origin;
    float tca = l.dot(ray.direction);
    if (tca < 0) return false;
    float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;

    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 0) t0 = t1;

    dist = (t0 * ray.direction).length();
    return true;
}
