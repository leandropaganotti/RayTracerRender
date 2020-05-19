#include "sphere.h"
#include "material.h"
#include "utils.h"

static std::shared_ptr<Sphere> unitSphere = std::shared_ptr<Sphere>(new Sphere);

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
Vector3 Sphere::normal(const Vector3 &phit, size_t) const
{
    return (phit-center).normalize();
}

//       analytic solution
//       Vec3f L = orig - center;
//       float a = dotProduct(dir, dir);
//       float b = 2 * dotProduct(dir, L);
//       float c = dotProduct(L, L) - radius2;
//       float t0, t1;
//       if (!solveQuadratic(a, b, c, t0, t1)) return false;
//       if (t0 < 0) t0 = t1;
//       if (t0 < 0) return false;
//       tnear = t0;
//       return true;
inline
Vector2 Sphere::uv(const Vector3 &phit, size_t) const
{
    Vector3 d = (phit-center).normalize();
    float u = 0.5 + atan2f(d.z, d.x) / (2.0f * M_PI);
    float v = 0.5 - asinf(d.y) / M_PI;
    return {u, v};
}

inline
void Sphere::fetch(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = (isec.phit - center).normalize();
}

AABB Sphere::getAABB() const
{
    return AABB(center + Vector3(-radius, -radius, radius), center + Vector3(radius, radius, -radius));
}

GSphere::GSphere(const Vector3 &c, const float &r): Sphere(c, r)
{
    material = Material::DiffuseWhite;
}

void GSphere::fetch(const Ray &ray, IntersectionData &isec) const
{
    Sphere::fetch(ray, isec);
    isec.material = material.get();
    isec.color = material->Kd;
    if(material->texture)
    {
        isec.uv = Sphere::uv(isec.phit, 0);
        isec.color = isec.color * material->texture->get(isec.uv);
    }
}

std::shared_ptr<Material> GSphere::getMaterial() const
{
    return material;
}

void GSphere::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : Material::DiffuseWhite;
}

GEllipsoid::GEllipsoid(): Instance(unitSphere){}

std::shared_ptr<Material> GEllipsoid::getMaterial() const
{
    return material;
}

void GEllipsoid::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : Material::DiffuseWhite;
}

void GEllipsoid::fetch(const Ray &ray, IntersectionData &isec) const
{
    Instance::fetch(ray, isec);
    isec.material = material.get();
    isec.color = material->Kd;
    if(material->texture)
    {
        isec.uv = static_cast<Sphere*>(shape.get())->uv(isec.phit_local, 0);
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
