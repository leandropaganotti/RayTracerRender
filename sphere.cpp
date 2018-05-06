#include "sphere.h"

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


Sphere::Sphere(const Vector3f &center, const float &radius, const Vector3f &color) :
    center(center), radius(radius), radius2(radius * radius)
{
    c_diffuse = color;
}

bool Sphere::intersection(const Ray &ray, IntersectionData &isec) const
{
    Vector3f l = center - ray.origin;
    float tca = l.dot(ray.direction);
    if (tca < 0) return false;
    float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;

    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    isec.tnear = t0 < 0 ? t1 : t0;
    //isec.phit = ray.origin + isec.tnear * ray.direction;
    //isec.normal = (isec.phit - center).normalize();
    isec.object = this;

    return true;
}

bool Sphere::intersection(const Ray &ray, float &tnear) const
{
    Vector3f l = center - ray.origin;
    float tca = l.dot(ray.direction);
    if (tca < 0) return false;
    float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;

    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    tnear = t0 < 0 ? t1 : t0;

    return true;
}
