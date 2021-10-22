#include "plane.h"
#include "ray.h"
#include "aabb.h"
#include "material.h"
#include "intersectiondata.h"

namespace shape {
    std::shared_ptr<Shape> XYPlane = std::make_shared<Plane>(vector::ZERO, vector::BACK);
}

Plane::Plane(const Vector3 &origin, const Vector3 &normal)
{
    const Vector3 n(1,0,0), m(0,1,0);
    this->origin = origin;
    w = normal;
    w.normalize();
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;
}



Vector3 Plane::getNormal() const
{
    return w;
}

void Plane::setNormal(const Vector3 &value)
{
    const Vector3 n(1,0,0), m(0,1,0);
    w = value;
    w.normalize();
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;
}

Vector3 Plane::getOrigin() const
{
    return origin;
}

void Plane::setOrigin(const Vector3 &value)
{
    origin = value;
}

bool Plane::intersection(const Ray &ray, IntersectionData& isec) const
{
    const float t = ((origin-ray.origin) ^ w) / (ray.direction ^ w);

    if( t < 0.0f || t > ray.tmax) return false;

    isec.tnear = t;
    isec.shape = this;
    return true;
}

bool Plane::intersection(const Ray &ray) const
{
    const float t = ((origin-ray.origin) ^ w) / (ray.direction ^ w);
    return ( t > 0.0f && t < ray.tmax) ? true : false;
}

void Plane::getNormalAndUV(IntersectionData &isec) const
{
    isec.normal = w;
    Vector3 p = isec.phit - origin;
    isec.uv.u = u^p;
    isec.uv.v = v^p;
}

inline
void Plane::getNormal(IntersectionData &isec) const
{
    isec.normal = w;
}

inline
void Plane::getUV(IntersectionData &isec) const
{
    Vector3 p = isec.phit - origin;
    isec.uv = Vector2(u^p, v^p);
}

AABB Plane::getAABB() const
{
    float t=1000.0;
    Vector3 P0 = origin - t * v;
    Vector3 P1 = origin + t * v;
    AABB aabb;

    aabb.extend(P0+0.1*w);
    aabb.extend(P1-0.1*w);

    P0 = origin - t * u;
    P1 = origin + t * u;

    aabb.extend(P0);
    aabb.extend(P1);

    return aabb;
}
