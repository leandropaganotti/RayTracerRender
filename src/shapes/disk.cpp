#include "disk.h"
#include "ray.h"
#include "intersectiondata.h"

Disk::Disk(const Vector3 &origin, const Vector3 &normal, float radius):Plane(origin, normal), radius(radius)
{

}

bool Disk::intersection(const Ray &ray, IntersectionData &isec) const
{
    IntersectionData isec_temp;

    if (Plane::intersection(ray, isec_temp)){
        Vector3 phit = ray.origin + isec_temp.tnear * ray.direction;
        float d = phit.distance(origin);
        if (d <= radius){
            isec = isec_temp;
            return true;
        }
    }
    return false;
}

bool Disk::intersection(const Ray &ray) const
{
    IntersectionData isec;
    return intersection(ray, isec);
}

AABB Disk::getAABB() const
{
    float t=radius;
    Vector3 P0 = origin - t * v;
    Vector3 P1 = origin + t * v;
    AABB aabb;

    aabb.extend(P0+0.1*w);
    aabb.extend(P1-0.1*w);

    P0 = origin - t * u;
    P1 = origin + t * u;

    aabb.extend(P0);
    aabb.extend(P1);

    return aabb;
}

void Disk::getSample(const Vector3 &from, Vector3 &dir, float &t, float &_1_pdf) const
{
    float r1=(dis(gen) * 2.0f * radius) - radius + origin.x;
    float r2=(dis(gen) * 2.0f * radius) - radius + origin.z;

    Vector3 p(r1, origin.y, r2);

    dir = p - from;
    if (dir.length2() == 0.f){
        _1_pdf = 0.0f;
    }
    else
    {
        dir.normalize();
        t = (p-from).length();
        _1_pdf = 1.0f / (4.0f * radius * radius);
        _1_pdf *= (t*t) / std::abs( w ^ -dir);
        if(std::isinf(_1_pdf)) _1_pdf = 0.f;
    }
}
