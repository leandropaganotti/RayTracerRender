#include "sphere.h"

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

Vector2 Sphere::uv(const Vector3 &phit, size_t) const
{
    Vector3 d = (phit-center).normalize();
    float u = 0.5 + atan2f(d.z, d.x) / (2.0f * M_PI);
    float v = 0.5 - asinf(d.y) / M_PI;
    return {u, v};
}

void Sphere::fetch(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = (isec.phit - center).normalize();
    isec.material = mat.get();

    if(mat->texture)
    {
        isec.uv.u = 0.5 + atan2f(isec.normal.z, isec.normal.x) / (2.0f * M_PI);
        isec.uv.v = 0.5 - asinf(isec.normal.y) / M_PI;
        isec.color = mat->texture->get(isec.uv) * mat->Kd;
    }
    else
        isec.color = mat->Kd;
}
