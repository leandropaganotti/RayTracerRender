#include "sphere.h"
#include <math.h>
#include "utils.h"

std::unique_ptr<Texture> &Sphere::getTex()
{
    return tex;
}

Sphere::Sphere(const Vector3f &center, const float &radius, const Vector3f &color) :
    Object(color), center(center), radius(radius), radius2(radius * radius)
{

}

Vector3f Sphere::getCenter() const
{
    return center;
}

void Sphere::setCenter(const Vector3f &value)
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

bool Sphere::intersection(const Ray &ray, IntersectionData &isec) const
{
    isec.object = this;
    return intersection(ray, isec.tnear);;
}

bool Sphere::intersection(const Ray &ray, float &tnear) const
{
    float t0, t1;

    // analytic solution
    Vector3f L = ray.origin - center;
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

const Vector3f Sphere::normal(const Vector3f &phit, size_t) const
{
    return (phit-center).normalize();
}

const Vector3f Sphere::texture(const Vector3f& phit, size_t) const
{
	if(tex)
	{
		Vector3f d = (phit-center).normalize();
		float u = 0.5 + atan2f(d.z, d.x) / (2.0f * PI);
		float v = 0.5 - asinf(d.y) / PI;
		return tex->get(u, v);
	}
	return Vector3f(1.0f);
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