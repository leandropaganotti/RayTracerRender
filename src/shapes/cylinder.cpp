#include "intersectiondata.h"
#include "cylinder.h"
#include "utils.h"
#include "material.h"
#include "float.h"

namespace primitives
{
    std::shared_ptr<Primitive> UnitCylinder = CreatePrimitive<UnitYCylinder>();
}

bool UnitYCylinder::intersection(const Ray &ray, IntersectionData &isec) const
{
    float a = ray.direction.x*ray.direction.x + ray.direction.z*ray.direction.z;
    float b = 2.0f*ray.direction.x*ray.origin.x + 2.0f*ray.direction.z*ray.origin.z;
    float c = ray.origin.x*ray.origin.x + ray.origin.z*ray.origin.z - r2;
    float t0, t1;

    int idx=-1;
    float t=ray.tmax;

    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);
    t=t0;
    if(t<0) t=t1;
    if(t>0)
    {
        float y = ray.origin.y + t * ray.direction.y;
        if (y<ymax && y>ymin)
        {
            idx=1;
        }
        else
        {
            t=ray.tmax;
        }
    }
    else
    {
        t=ray.tmax;
    }


    float t2 = (ymax - ray.origin.y) / ray.direction.y;
    if(t2 > 0){
        float x = ray.origin.x + t2 * ray.direction.x;
        float z = ray.origin.z + t2 * ray.direction.z;
        if (x*x + z*z <= r2) {
            if(t2 < t)
            {
                t=t2;
                idx=2;
            }
        }
    }

    float t3 = (-ymin + ray.origin.y) / -ray.direction.y;
    if(t3 > 0){
        float x = ray.origin.x + t3 * ray.direction.x;
        float z = ray.origin.z + t3 * ray.direction.z;
        if (x*x + z*z <= r2) {
            if(t3 < t)
            {
                t=t3;
                idx=0;
            }
        }
    }

    if(idx>=0 && t < ray.tmax)
    {
        isec.tnear =t;
        isec.idx = idx;
        isec.primitive = this;
        return true;
    }
    return false;
}

bool UnitYCylinder::intersection(const Ray &ray) const
{
    /*
     * x2 + y2 = 1
     * P(t) = E + tD
     * (xE + t.xD)2 + (yE + t.yD)2 = 1
     * a.t2 + b.t + c = 0, where
     * a = xD2 + yD2
     * b = 2.xD.xE + 2.yD.yE
     * c = xE2 + yE2 - 1
    */
    float a = ray.direction.x*ray.direction.x + ray.direction.z*ray.direction.z;
    float b = 2.0f*ray.direction.x*ray.origin.x + 2.0f*ray.direction.z*ray.origin.z;
    float c = ray.origin.x*ray.origin.x + ray.origin.z*ray.origin.z - r2;
    float t0, t1;

    int idx=-1;
    float t=ray.tmax;

    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);
    t=t0;
    if(t<0) t=t1;
    if(t>0)
    {
        float y = ray.origin.y + t * ray.direction.y;
        if (y<ymax && y>ymin)
        {
            idx=1;
        }
        else
        {
            t=ray.tmax;
        }
    }
    else
    {
        t=ray.tmax;
    }

    float t2 = (ymax - ray.origin.y) / ray.direction.y;
    if(t2 > 0){
        float x = ray.origin.x + t2 * ray.direction.x;
        float z = ray.origin.z + t2 * ray.direction.z;
        if (x*x + z*z <= r2) {
            if(t2 < t)
            {
                t=t2;
                idx=2;
            }
        }
    }

    float t3 = (-ymin + ray.origin.y) / -ray.direction.y;
    if(t3 > 0){
        float x = ray.origin.x + t3 * ray.direction.x;
        float z = ray.origin.z + t3 * ray.direction.z;
        if (x*x + z*z <= r2) {
            if(t3 < t)
            {
                t=t3;
                idx=0;
            }
        }
    }

    if(idx>=0 && t < ray.tmax)
    {
        return true;
    }
    return false;
}

void UnitYCylinder::getNormalAndUV(IntersectionData &isec) const
{
    if(isec.idx==0)
    {
        isec.normal = vector::DOWN;
        isec.uv.u = isec.phit.x + 0.5;
        isec.uv.v = isec.phit.z + 0.5;
    }
    else if(isec.idx==1)
    {
        isec.normal = Vector3(isec.phit.x, 0.0f, isec.phit.z) / r;
        isec.uv.u = atan2(isec.phit.x, isec.phit.z) / (2*M_PI) +0.5;
        isec.uv.v = 1.0f - isec.phit.y + 0.5f;
    }
    else
    {
        isec.normal = vector::UP;
        isec.uv.u = isec.phit.x + 0.5;
        isec.uv.v = isec.phit.z + 0.5;
    }
}

inline
void UnitYCylinder::getNormal(IntersectionData &isec) const
{    
    if(isec.idx==0)
    {
        isec.normal = vector::DOWN;
    }
    else if(isec.idx==1)
    {
        isec.normal = Vector3(isec.phit.x, 0.0f, isec.phit.z) / r;
    }
    else
    {
        isec.normal = vector::UP;
    }
}
inline
void UnitYCylinder::getUV(IntersectionData &isec) const
{
    if(isec.idx==0)
    {
        isec.uv.u = isec.phit.x + 0.5;
        isec.uv.v = isec.phit.z + 0.5;
    }
    else if(isec.idx==1)
    {
        isec.uv.u = atan2(isec.phit.x, isec.phit.z) / (2*M_PI) +0.5;
        isec.uv.v = 1.0f - isec.phit.y + 0.5f;
    }
    else
    {
        isec.uv.u = isec.phit.x + 0.5;
        isec.uv.v = isec.phit.z + 0.5;
    }
}

AABB UnitYCylinder::getAABB() const
{
    return AABB({-0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, 0.5f});
}
