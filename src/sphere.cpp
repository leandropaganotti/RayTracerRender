#include "sphere.h"
#include <math.h>
#include "utils.h"

Sphere::Sphere(const Vector3f &center, const float &radius, const Vector3f &color) :
    Object(color), center(center), radius(radius), radius2(radius * radius)
{
	//tex.reset(new ChessBoard(0.1, 0.2, {1,1,0 }, {1, 0.5, 0.6}));
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
    Vector3f l = center - ray.origin;
    float tca = l.dot(ray.direction);
    if (tca < 0) return false;
    float d2 = l.dot(l) - tca * tca;
    if (d2 > radius2) return false;

    float thc = sqrt(radius2 - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    isec.tnear = t0 < 0 ? t1 : t0;
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

const Vector3f Sphere::normal(const Vector3f &phit, size_t) const
{
    return (phit-center).normalize();
}

const Vector3f Sphere::texture(const Vector3f& phit) const
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
