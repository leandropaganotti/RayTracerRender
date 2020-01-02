#include "cylinder.h"
#include "utils.h"
#include "consts.h"

UnitCylinder::UnitCylinder()
{

}

bool UnitCylinder::intersection(const Ray &ray, IntersectionData &isec) const
{
    float a = ray.direction.x*ray.direction.x + ray.direction.z*ray.direction.z;
    float b = 2.0f*ray.direction.x*ray.origin.x + 2.0f*ray.direction.z*ray.origin.z;
    float c = ray.origin.x*ray.origin.x + ray.origin.z*ray.origin.z - 1.0f;
    float t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    float y0 = ray.origin.y + t0 * ray.direction.y;
    float y1 = ray.origin.y + t1 * ray.direction.y;

    if (y0<0)
    {
        if (y1<0)
            return false;
        else
        {
            // hit the cap
            isec.tnear = t0 + (t1-t0) * (y0+1) / (y0-y1);
            if (isec.tnear<=0) return false;
            isec.idx=0;
            return true;
        }
    }
    else if (y0>=0 && y0<=1)
    {
        // hit the cylinder bit
        if (t0<=0) return false;
        isec.tnear=t0;
        isec.idx=1;
        return true;
    }
    else if (y0>1)
    {
        if (y1>1)
            return false;
        else
        {
            // hit the cap
            isec.tnear = t0 + (t1-t0) * (y0-1) / (y0-y1);
            if (isec.tnear<=0) return false;
            isec.idx=2;
            return true;
        }
    }

    return false;
}

bool UnitCylinder::intersection(const Ray &ray, float &tnear) const
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
    float c = ray.origin.x*ray.origin.x + ray.origin.z*ray.origin.z - 1.0f;
    float t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    float y0 = ray.origin.y + t0 * ray.direction.y;
    float y1 = ray.origin.y + t1 * ray.direction.y;

    if (y0<0)
    {
        if (y1<0)
            return false;
        else
        {
            // hit the cap
            tnear = t0 + (t1-t0) * (y0+1) / (y0-y1);
            if (tnear<=0) return false;
            return true;
        }
    }
    else if (y0>=0 && y0<=1)
    {
        // hit the cylinder bit
        if (t0<=0) return false;
        tnear=t0;
        return true;
    }
    else if (y0>1)
    {
        if (y1>1)
            return false;
        else
        {
            // hit the cap
            tnear = t0 + (t1-t0) * (y0-1) / (y0-y1);
            if (tnear<=0) return false;
            return true;
        }
    }

    return false;
}

Vector3 UnitCylinder::normal(const Vector3 &phit, size_t idx) const
{    
    if(idx==0)
    {
        return Vector::DOWN;
    }
    else if(idx==1)
    {
        return Vector3(phit.x, 0.0f, phit.z).normalize();
    }
    else
    {
        return Vector::UP;
    }
}

std::pair<float, float> UnitCylinder::uv(const Vector3 &, size_t) const
{
    return std::pair<float, float>(0.0f, 0.0f);
}

Cylinder::Cylinder()
{
    shape = std::shared_ptr<Shape>(new UnitCylinder());
}
