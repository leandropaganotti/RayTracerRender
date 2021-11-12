#include "sphere.h"
#include "material.h"
#include "utils.h"
#include "intersectiondata.h"

namespace primitives
{
    std::shared_ptr<Primitive> UnitSphere = CreatePrimitive<Sphere>(0.0f, 0.5f);
}

Sphere::Sphere(const Vector3 &center, const float &radius) :
    center(center), radius(radius), radius2(radius * radius)
{

}

Vector3 Sphere::getCenter() const
{
    return center;
}

void Sphere::setCenter(const Vector3 &value)
{
    center = value;
}

float Sphere::getRadius() const
{
    return radius;
}

void Sphere::setRadius(float value)
{
    radius = value;
    radius2 = radius * radius;
}

inline
bool Sphere::intersection(const Ray &ray, IntersectionData &isec) const
{
    float t0, t1;

    // analytic solution
    Vector3 L = ray.origin - center;
    float a = ray.direction.dot(ray.direction);
    float b = 2 * ray.direction.dot(L);
    float c = L.dot(L) - radius2;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) return false;
    }
    if(t0 > ray.tmax) return false;

    isec.tnear = t0;
    isec.primitive = this;
    return true;
}

inline
bool Sphere::intersection(const Ray &ray) const
{
    float t0, t1;

    // analytic solution
    Vector3 L = ray.origin - center;
    float a = ray.direction.dot(ray.direction);
    float b = 2 * ray.direction.dot(L);
    float c = L.dot(L) - radius2;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) return false;
    }
    return t0 < ray.tmax ? true : false;
}

void Sphere::getNormalAndUV(IntersectionData &isec) const
{
    isec.normal = (isec.phit - center) / radius;
    isec.uv.u = 0.5 + atan2f(isec.normal.x, isec.normal.z) / (2.0f * M_PI);
    isec.uv.v = 0.5 - asinf(isec.normal.y) / M_PI;
}

inline
void Sphere::getNormal(IntersectionData &isec) const
{
    isec.normal = (isec.phit-center) / radius;
}

inline
void Sphere::getUV(IntersectionData &isec) const
{
    Vector3 n = (isec.phit-center) / radius;
    isec.uv.u = 0.5 + atan2f(n.x, n.z) / (2.0f * M_PI);
    isec.uv.v = 0.5 - asinf(n.y) / M_PI;
}

AABB Sphere::getAABB() const
{
    return AABB(center + Vector3(-radius, -radius, radius), center + Vector3(radius, radius, -radius));
}

void Sphere::getSample(const Vector3 &from, Vector3 &dir, float &t, float &_1_pdf) const
{
    dir = sampleSolidAngleSphere(center, radius, from, _1_pdf);
    IntersectionData isec;
    intersection(Ray(from, dir), isec);
    t = isec.tnear;
}
