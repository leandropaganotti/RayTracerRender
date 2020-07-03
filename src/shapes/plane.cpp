#include "plane.h"
#include "ray.h"
#include "aabb.h"
#include "material.h"

Plane::Plane(const Vector3 &origin, const Vector3 &normal):
    origin(origin), w(normal)
{
    this->w.normalize();
    const Vector3 n(0,1,0), m(1,0,0);
    this->v = w%n; if(this->v.length()<0.01f)this->v = w%m;
    this->u=normal%this->v;
}

Plane::~Plane(){}

Vector3 Plane::getNormal() const
{
    return w;
}

void Plane::setNormal(const Vector3 &value)
{
    w = value;
    w.normalize();
    const Vector3 n(0,1,0), m(1,0,0);
    v = w%n; if(v.length()<0.01f)v = w%m;
    u=w%v;
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

void Plane::getIsecData(IntersectionData &isec) const
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
