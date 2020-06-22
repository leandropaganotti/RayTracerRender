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

bool Plane::intersection(const Ray &ray, float tmax, IntersectionData& isec) const
{
    const float t = ((origin-ray.origin) ^ w) / (ray.direction ^ w);

    if( t < 0.0f || t > tmax) return false;

    isec.tnear = t;
    isec.shape = this;
    return true;
}

bool Plane::intersection(const Ray &ray, float tmax) const
{
    const float t = ((origin-ray.origin) ^ w) / (ray.direction ^ w);
    return ( t > 0.0f && t < tmax) ? true : false;
}

inline
void Plane::getNormal(IntersectionData &isec) const
{
    isec.normal = w;
}

inline
Vector2 Plane::getUV(const Vector3 &phit, size_t) const
{
    Vector3 p = phit - origin;
    return Vector2(u^p, v^p);
}

AABB Plane::getAABB() const
{
    Vector3 v = Vector3(1,0,0) % w;
    if(v.length() < 0.1f) v = Vector3(0,1,0) % w;

    float t=1000.0;
    Vector3 P0 = origin - t * v.normalize();
    Vector3 P1 = origin + t * v.normalize();
    AABB aabb;

    aabb.extend(P0+0.1*w);
    aabb.extend(P1-0.1*w);

    v = v % w;
    P0 = origin - t * v.normalize();
    P1 = origin + t * v.normalize();

    aabb.extend(P0);
    aabb.extend(P1);

    return aabb;
}

GPlane::GPlane(const Vector3 &origin, const Vector3 &normal): Plane(origin, normal)
{
    material = material::DiffuseWhite;
}

GPlane::~GPlane() {}

void GPlane::getIsecData(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = w;
    isec.material = material.get();
    isec.color = material->Kd;

    if(material->texture)
    {
        isec.uv = Plane::getUV(isec.phit, 0);
        isec.color = material->texture->get(isec.uv) * material->Kd;
    }
}

void GPlane::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : material::DiffuseWhite;
}

const Material *GPlane::getMaterial(size_t) const
{
    return material.get();
}
