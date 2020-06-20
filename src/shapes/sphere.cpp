#include "sphere.h"
#include "material.h"
#include "utils.h"

static std::shared_ptr<Sphere> unitSphere = std::shared_ptr<Sphere>(new Sphere(0.0f, 0.5f));

Sphere::Sphere(const Vector3 &center, const float &radius) :
    center(center), radius(radius), radius2(radius * radius)
{

}

Sphere::~Sphere(){}

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
bool Sphere::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
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
    if(t0 > tmax) return false;

    isec.tnear = t0;
    isec.shape = this;
    return true;
}

inline
bool Sphere::intersection(const Ray &ray, float tmax) const
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
    return t0 < tmax ? true : false;
}

inline
Vector3 Sphere::getNormal(const Vector3 &phit, size_t) const
{
    return (phit-center) / radius;
}

inline
Vector2 Sphere::getUV(const Vector3 &phit, size_t) const
{
    Vector3 n = (phit-center) / radius;
    float u = 0.5 + atan2f(n.x, n.z) / (2.0f * M_PI);
    float v = 0.5 - asinf(n.y) / M_PI;
    return {u, v};
}

AABB Sphere::getAABB() const
{
    return AABB(center + Vector3(-radius, -radius, radius), center + Vector3(radius, radius, -radius));
}

GSphere::GSphere(const Vector3 &c, const float &r): Sphere(c, r)
{
    material = material::DiffuseWhite;
}

void GSphere::getIsecData(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = (isec.phit - center).normalize();
    isec.material = material.get();
    isec.color = material->Kd;
    if(material->texture)
    {
        isec.uv = Sphere::getUV(isec.phit, 0);
        isec.color = isec.color * material->texture->get(isec.uv);
    }
}

void GSphere::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : material::DiffuseWhite;
}

const Material *GSphere::getMaterial(size_t) const
{
    return material.get();
}

GEllipsoid::GEllipsoid(): Instance(unitSphere)
{
    material = material::DiffuseWhite;
}

void GEllipsoid::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : material::DiffuseWhite;
}

const Material *GEllipsoid::getMaterial(size_t) const
{
    return material.get();
}

void GEllipsoid::getIsecData(const Ray &ray, IntersectionData &isec) const
{
    Instance::getIsecData(ray, isec);
    isec.material = material.get();
    isec.color = material->Kd;
    if(material->texture)
    {
        isec.uv = static_cast<Sphere*>(shape.get())->getUV(isec.phit_local, 0);
        isec.color = isec.color * material->texture->get(isec.uv);
    }
}

void Sphere::sampleSolidAngleSphere(const Vector3 &point, Vector3 &sample, float &_1_pdf) const{
    float dist2 = (center - point).length(); dist2*=dist2; // distance from point hit to center of light power 2
    float cos_a_max = sqrt(1.0f - radius2/dist2);
    float r1 = dis(gen);
    float r2 = dis(gen);
    float cos_a = 1.0f + r1*(cos_a_max-1.0f);
    float phi = 2.0f * M_PI * r2;
    //float sin_a = sqrtf(1-cos_a*cos_a);
    float sin_a = sinf(acosf(cos_a));
    Vector3 u,v, w=(center - point).normalize(), n(1,0,0),m(0,1,0);
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;
    sample = u*(cos(phi)*sin_a) + v*(sin(phi)*sin_a) + w*(cos_a);

    _1_pdf = (2.0f*M_PI*(1.0f-cos_a_max));//1/(2*M_PI*(1-cos_a_max));
}
