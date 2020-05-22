#include "plane.h"
#include "ray.h"
#include "aabb.h"
#include "material.h"

Plane::Plane(const Vector3 &O, const Vector3 &N):
    O(O), N(N)
{

}

Plane::~Plane(){}

Vector3 Plane::getN() const
{
    return N;
}

void Plane::setN(const Vector3 &value)
{
    N = value;
}

Vector3 Plane::getO() const
{
    return O;
}

void Plane::setO(const Vector3 &value)
{
    O = value;
}

bool Plane::intersection(const Ray &ray, float tmax, IntersectionData& isec) const
{
    const float t = ((O-ray.origin) ^ N) / (ray.direction ^ N);

    if( t < 0.0f || t > tmax) return false;

    isec.tnear = t;
    isec.shape = this;
    return true;
}

bool Plane::intersection(const Ray &ray, float tmax) const
{
    const float t = ((O-ray.origin) ^ N) / (ray.direction ^ N);
    return ( t > 0.0f && t < tmax) ? true : false;
}

inline
Vector3 Plane::getNormal(const Vector3 &, size_t) const
{
    return N;
}

inline
Vector2 Plane::getUV(const Vector3 &phit, size_t) const
{
    const float e=0.001f;
    const Vector3 v = phit - O;

    if(N.x > e || N.x < -e) return {v.y, v.z};

    if(N.y > e || N.y < -e) return {v.x, v.z};

    return {v.x, v.y};
}

AABB Plane::getAABB() const
{
    Vector3 v = Vector3(1,0,0) % N;
    if(v.length() < 0.1) v = Vector3(0,1,0) % N;

    float t=1000.0;
    Vector3 P0 = O - t * v.normalize();
    Vector3 P1 = O + t * v.normalize();
    AABB aabb;

    aabb.extend(P0+0.1*N);
    aabb.extend(P1-0.1*N);

    v = v % N;
    P0 = O - t * v.normalize();
    P1 = O + t * v.normalize();

    aabb.extend(P0);
    aabb.extend(P1);

    return aabb;
}

GPlane::GPlane(const Vector3 &o, const Vector3 &n): Plane(o, n)
{
    material = Material::DiffuseWhite;
}

GPlane::~GPlane() {}

void GPlane::getIsecData(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = N;
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
    material = value ? value : Material::DiffuseWhite;
}

const Material *GPlane::getMaterial(size_t) const
{
    return material.get();
}
