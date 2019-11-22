#include "sphere.h"

GEOMSphere::GEOMSphere(const Vector3 &center, const float &radius) :
    center(center), radius(radius), radius2(radius * radius)
{

}

Vector3 GEOMSphere::getCenter() const
{
    return center;
}

void GEOMSphere::setCenter(const Vector3 &value)
{
    center = value;
}

float GEOMSphere::getRadius() const
{
    return radius;
}

void GEOMSphere::setRadius(float value)
{
    radius = value;
    radius2 = radius * radius;
}

bool GEOMSphere::intersection(const Ray &ray, IntersectionData &isec) const
{    
    return intersection(ray, isec.tnear);;
}

bool GEOMSphere::intersection(const Ray &ray, float &tnear) const
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
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) return false; // both t0 and t1 are negative
    }
    tnear = t0;
    return true;
}

const Vector3 GEOMSphere::normal(const Vector3 &phit, size_t) const
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



