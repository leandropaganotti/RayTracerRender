#include "cylinder.h"
#include "utils.h"
#include "consts.h"
#include "material.h"
#include "float.h"

static std::shared_ptr<UnitYCylinder> unitCylinder = std::shared_ptr<UnitYCylinder>(new UnitYCylinder);

UnitYCylinder::UnitYCylinder()
{
    r = 0.5f;
    r2 = r*r;
    ymin = -0.5f;
    ymax = 0.5;
}

bool UnitYCylinder::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
{
    float a = ray.direction.x*ray.direction.x + ray.direction.z*ray.direction.z;
    float b = 2.0f*ray.direction.x*ray.origin.x + 2.0f*ray.direction.z*ray.origin.z;
    float c = ray.origin.x*ray.origin.x + ray.origin.z*ray.origin.z - r2;
    float t0, t1;

    int idx=-1;
    float t=FLT_MAX;

    if (solveQuadratic(a, b, c, t0, t1))
    {
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
                t=FLT_MAX;
            }
        }
        else
        {
            t=FLT_MAX;
        }
    }


//    float y0 = ray.origin.y + t0 * ray.direction.y;
//    float y1 = ray.origin.y + t1 * ray.direction.y;

//    if(y0 < ymin && y1 < ymin) return false;
//    if(y0 > ymax && y1 > ymax) return false;

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

    if(idx>=0)
    {
        isec.tnear =t;
        isec.idx = idx;
        return true;
    }
    return false;
}

bool UnitYCylinder::intersection(const Ray &ray, float tmax) const
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
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1) std::swap(t0, t1);

    float y0 = ray.origin.y + t0 * ray.direction.y;
    float y1 = ray.origin.y + t1 * ray.direction.y;

    if (y0<ymin)
    {
        if (y1<ymin)
            return false;
        else
        {
            // hit the cap
            const float tval = t0 + (t1-t0) * (y0+1) / (y0-y1);
            if (tval<=0 || tval > tmax) return false;
            return true;
        }
    }
    else if (y0>=ymin && y0<=ymax)
    {
        // hit the cylinder bit
        if (t0<=ymin || t0 > tmax) return false;
        return true;
    }
    else if (y0>ymax)
    {
        if (y1>ymax)
            return false;
        else
        {
            // hit the cap
            const float tval = t0 + (t1-t0) * (y0-1) / (y0-y1);
            if (tval<=0 || tval > tmax) return false;
            return true;
        }
    }

    return false;
}

inline
Vector3 UnitYCylinder::getNormal(const Vector3 &phit, size_t idx) const
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

AABB UnitYCylinder::getAABB() const
{
    return AABB({-1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, -1.0f});
}

GCylinder::GCylinder(): Instance(unitCylinder)
{
    material = material::DiffuseWhite;
}

void GCylinder::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : material::DiffuseWhite;
}

const Material *GCylinder::getMaterial(size_t) const
{
    return material.get();
}

void GCylinder::getIsecData(const Ray &ray, IntersectionData &isec) const
{
    Instance::getIsecData(ray, isec);
    isec.material = material.get();
    isec.color = material->Kd;
}
