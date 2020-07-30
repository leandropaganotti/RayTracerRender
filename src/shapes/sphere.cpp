#include "sphere.h"
#include "material.h"
#include "utils.h"

namespace shape
{
    std::shared_ptr<Shape> unitSphere = CreateShape<Sphere>(0.0f, 0.5f);
}

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
    isec.shape = this;
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

void Sphere::getIsecData(IntersectionData &isec) const
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
inline Vector3 SphericalDirection(float sinTheta, float cosTheta, float phi,
                                   const Vector3 &x, const Vector3 &y,
                                   const Vector3 &z) {
    return sinTheta * std::cos(phi) * x + sinTheta * std::sin(phi) * y +
           cosTheta * z;
}
void Sphere::getSample(const Vector3 &point, Vector3 &sample, float &_1_pdf) const
{
    static RNG rng;
    float r1 = rng();
    float r2 = rng();

    // Compute coordinate system for sphere sampling
    float dc = point.distance(center);
    float invDc = 1 / dc;
    Vector3 w = (center - point) * invDc;
    Vector3 u, v;
    Vector3 n(1,0,0),m(0,1,0);
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;

    // Compute $\theta$ and $\phi$ values for sample in cone
    float sinThetaMax = radius * invDc;
    float sinThetaMax2 = sinThetaMax * sinThetaMax;
    float invSinThetaMax = 1 / sinThetaMax;
    float cosThetaMax = std::sqrt(std::max((float)0.f, 1 - sinThetaMax2));

    float cosTheta  = (cosThetaMax - 1) * r1 + 1;
    float sinTheta2 = 1 - cosTheta * cosTheta;

    if (sinThetaMax2 < 0.00068523f /* sin^2(1.5 deg) */) {
       /* Fall back to a Taylor series expansion for small angles, where
          the standard approach suffers from severe cancellation errors */
       sinTheta2 = sinThetaMax2 * r1;
       cosTheta = std::sqrt(1 - sinTheta2);
    }

    // Compute angle $\alpha$ from center of sphere to sampled point on surface
    float cosAlpha = sinTheta2 * invSinThetaMax +
       cosTheta * std::sqrt(std::max((float)0.f, 1.f - sinTheta2 * invSinThetaMax * invSinThetaMax));
    float sinAlpha = std::sqrt(std::max((float)0.f, 1.f - cosAlpha*cosAlpha));
    float phi = r2 * 2 * M_PI;

    // Compute surface normal and sampled point on sphere
    Vector3 nWorld = SphericalDirection(sinAlpha, cosAlpha, phi, -u, -v, -w);
    Vector3 pWorld = center + radius * Vector3(nWorld.x, nWorld.y, nWorld.z);

    sample = (pWorld-point).normalize();

    _1_pdf = (2 * M_PI * (1 - cosThetaMax));
}

void Sphere::getSample(IntersectionData &isec, ShapeSample &ss) const
{
    static RNG rng;
    float r1 = rng();
    float r2 = rng();

    // Compute coordinate system for sphere sampling
    float dc = isec.phit.distance(center);
    float invDc = 1 / dc;
    Vector3 w = (center - isec.phit) * invDc;
    Vector3 u, v;
    Vector3 n(1,0,0),m(0,1,0);
    u = w%n; if(u.length()<0.01f)u = w%m;
    v=w%u;

    // Compute $\theta$ and $\phi$ values for sample in cone
    float sinThetaMax = radius * invDc;
    float sinThetaMax2 = sinThetaMax * sinThetaMax;
    float invSinThetaMax = 1 / sinThetaMax;
    float cosThetaMax = std::sqrt(std::max((float)0.f, 1 - sinThetaMax2));

    float cosTheta  = (cosThetaMax - 1) * r1 + 1;
    float sinTheta2 = 1 - cosTheta * cosTheta;

    if (sinThetaMax2 < 0.00068523f /* sin^2(1.5 deg) */) {
       /* Fall back to a Taylor series expansion for small angles, where
          the standard approach suffers from severe cancellation errors */
       sinTheta2 = sinThetaMax2 * r1;
       cosTheta = std::sqrt(1 - sinTheta2);
    }

    // Compute angle $\alpha$ from center of sphere to sampled point on surface
    float cosAlpha = sinTheta2 * invSinThetaMax +
       cosTheta * std::sqrt(std::max((float)0.f, 1.f - sinTheta2 * invSinThetaMax * invSinThetaMax));
    float sinAlpha = std::sqrt(std::max((float)0.f, 1.f - cosAlpha*cosAlpha));
    float phi = r2 * 2 * M_PI;

    // Compute surface normal and sampled point on sphere
    ss.n = SphericalDirection(sinAlpha, cosAlpha, phi, -u, -v, -w);
    ss.p = center + radius * ss.n;
    ss._1_pdf = (2 * M_PI * (1 - cosThetaMax));
}
