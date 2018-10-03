#include "box.h"
#include <float.h>
#include <stdio.h>      /* printf */
#include <math.h>
#include "utils.h"
#include <utility>

Box::Box() : min(-0.5f, -0.5f, 0.5f), max(0.5f,0.5f,-0.5f)
{
    material.kd = 1;
    material.type = Material::Type::DIFFUSE;
}

bool Box::intersection(const Ray &ray, IntersectionData &isec) const
{
    Ray r = getInverse() * ray;

    Vector3 invdir = 1.0f / r.direction ;

    float t1 = (min.x - r.origin.x)*invdir.x;
    float t2 = (max.x - r.origin.x)*invdir.x;
    float t3 = (min.y - r.origin.y)*invdir.y;
    float t4 = (max.y - r.origin.y)*invdir.y;
    float t5 = (min.z - r.origin.z)*invdir.z;
    float t6 = (max.z - r.origin.z)*invdir.z;

    float tminx, tminy, tminz, tmin;
    size_t idxx, idxy, idxz;

    if(t1 < t2) { idxx = 1; tminx = t1; } else { idxx = 2; tminx = t2; }

    if(t3 < t4) { idxy = 3; tminy = t3; } else { idxy = 4; tminy = t4; }

    if(t5 < t6) { idxz = 5; tminz = t5; } else { idxz = 6; tminz = t6; }

    if ( tminx > tminy ) { tmin = tminx; isec.idx = idxx; } else { tmin = tminy; isec.idx = idxy; }

    if ( tminz > tmin ) { tmin = tminz; isec.idx = idxz; }

    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    // if tmax < 0, it is behind the ray
    if (tmax < 0) { return false; }

    // if tmin > tmax, ray doesn't intersect
    if (tmin > tmax) { return false; }

    // if tmin < 0 then the ray origin is inside of the box and tmin is behind the start of the ray so tmax is the first intersection
    if(tmin < 0) { isec.tnear = tmax; } else { isec.tnear = tmin; }

    Vector3 phit = r.origin + isec.tnear * r.direction;
    phit = getModelMatrix() * phit;
    isec.tnear = (phit - ray.origin).length();
    isec.object = this;
    return true;
}

bool Box::intersection(const Ray &ray, float &tnear) const
{
    Ray r = getInverse() * ray;

    Vector3 invdir = 1.0f / r.direction;

    float t1 = (min.x - r.origin.x)*invdir.x;
    float t2 = (max.x - r.origin.x)*invdir.x;
    float t3 = (min.y - r.origin.y)*invdir.y;
    float t4 = (max.y - r.origin.y)*invdir.y;
    float t5 = (min.z - r.origin.z)*invdir.z;
    float t6 = (max.z - r.origin.z)*invdir.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax < 0 || tmin > tmax) { return false; }

    if(tmin < 0) { tnear = tmax; } else { tnear = tmin; }

    Vector3 phit = r.origin + tnear * r.direction;
    phit = getModelMatrix() * phit;
    tnear = (phit - ray.origin).length();

    return true;
}

const Vector3 Box::normal(const Vector3 &, size_t idx) const
{
    if (idx == 1)      return (getInverseTranspose() * Vector3(-1,0,0)).normalize();
    else if (idx == 2) return (getInverseTranspose() * Vector3(1,0,0)).normalize();
    else if (idx == 3) return (getInverseTranspose() * Vector3(0,-1,0)).normalize();
    else if (idx == 4) return (getInverseTranspose() * Vector3(0,1,0)).normalize();
    else if (idx == 5) return (getInverseTranspose() * Vector3(0,0,1)).normalize();
    else               return (getInverseTranspose() * Vector3(0,0,-1)).normalize();
}

const std::pair<float, float> Box::texUV(const Vector3 &phit, size_t idx) const
{
    float u=0, v=0;
    Vector3 p = getInverse() * phit;

    if (idx == 1 || idx == 2)
    {
        u = p.z; v = p.y;
    } else if (idx == 3 || idx == 4)
    {
        u = p.x; v = p.z;
    } else
    {
        u = p.x; v = p.y;
    }

    return std::make_pair(u, v);
}
