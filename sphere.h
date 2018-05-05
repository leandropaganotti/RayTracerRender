#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class Sphere: public Object
{
public:
    Vector3f center;                        // position of the sphere
    float radius, radius2;                  // sphere radius and radius^2

    Sphere( const Vector3f &center={0.0f}, const float &radius=1.0f, const Vector3f &color={1.0f});

    bool  intersection(const Ray &ray, IntersectionData &inter) const;

    bool  intersection(const Ray& ray, float& tNear) const;
};

#endif // SPHERE_H

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
