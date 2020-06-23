#include "box.h"
#include "material.h"
#include "float.h"

static std::shared_ptr<AABox> unitBox = std::shared_ptr<AABox>(new AABox);

AABox::AABox(const Vector3 &min, const Vector3 &max): data{min, max}
{
}

Vector3 AABox::getMax() const
{
    return data[1];
}

void AABox::setMax(const Vector3 &value)
{
    data[1] = value;
}

bool AABox::intersection(const Ray &ray, IntersectionData &isec) const
{
    Ray r = ray;
    r.invdir = 1.0f / ray.direction;
    r.posneg[0] = r.direction[0] > 0 ? 0 : 1;
    r.posneg[1] = r.direction[1] > 0 ? 0 : 1;
    r.posneg[2] = r.direction[2] > 0 ? 0 : 1;

    float imin = -INFINITY  ;
    float imax = ray.tmax;
    int idxmin=0, idxmax=0;

    int posneg = r.posneg[0];
    float t0 = (data[posneg].x - r.origin.x) * r.invdir.x;
    float t1 = (data[1-posneg].x - r.origin.x) * r.invdir.x;
    if(t0 > imin) {imin = t0;idxmin=posneg;}
    if(t1 < imax) {imax = t1;idxmax=1-posneg;}
    if(imin > imax) return false;

    posneg = r.posneg[1];
    t0 = (data[posneg].y - r.origin.y) * r.invdir.y;
    t1 = (data[1-posneg].y - r.origin.y) * r.invdir.y;
    if(t0 > imin) {imin = t0;idxmin=2+posneg;}
    if(t1 < imax) {imax = t1;idxmax=2+1-posneg;}
    if(imin > imax) return false;

    posneg = r.posneg[2];
    t0 = (data[posneg].z - r.origin.z) * r.invdir.z;
    t1 = (data[1-posneg].z - r.origin.z) * r.invdir.z;
    if(t0 > imin) {imin = t0;idxmin=4+posneg;}
    if(t1 < imax) {imax = t1;idxmax=4+1-posneg;}
    if(imin<0 && imax>0)
    {
        isec.tnear = imax;
        isec.idx=idxmax;
        return true;
    }
    if(imin>0 && imin <= imax)
    {
        isec.tnear = imin;
        isec.idx=idxmin;
        return true;
    }

    return  false;
}

bool AABox::intersection(const Ray &ray) const
{
    Vector3 invdir = 1.0f / ray.direction;

    float t1 = (data[0].x - ray.origin.x)*invdir.x;
    float t2 = (data[1].x - ray.origin.x)*invdir.x;
    float t3 = (data[0].y - ray.origin.y)*invdir.y;
    float t4 = (data[1].y - ray.origin.y)*invdir.y;
    float t5 = (data[0].z - ray.origin.z)*invdir.z;
    float t6 = (data[1].z - ray.origin.z)*invdir.z;

    float tmin_ = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax_ = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax_ < 0 || tmin_ > tmax_) { return false; }

    float tval;
    if(tmin_ < 0) { tval = tmax_; } else { tval = tmin_; }

    if (tval > ray.tmax) return false;

    return true;
}

inline
void AABox::getNormal(IntersectionData &isec) const
{
    if (isec.idx == 0)      isec.normal = vector::LEFT;
    else if (isec.idx == 1) isec.normal = vector::RIGHT;
    else if (isec.idx == 2) isec.normal = vector::DOWN;
    else if (isec.idx == 3) isec.normal = vector::UP;
    else if (isec.idx == 4) isec.normal = vector::FRONT;
    else                    isec.normal = vector::BACK;
}

inline
void AABox::getUV(IntersectionData &isec) const
{
    Vector3 p = isec.phit + 0.5f;

    if (isec.idx == 0)      isec.uv = Vector2(p.z, 1.0f-p.y);
    else if (isec.idx == 1) isec.uv = Vector2(1.0f-p.z, 1.0f-p.y);
    else if (isec.idx == 2) isec.uv = Vector2(p.x, 1.0f-p.z);
    else if (isec.idx == 3) isec.uv = Vector2(p.x, p.z);
    else if (isec.idx == 4) isec.uv = Vector2(1.0f-p.x, 1.0f-p.y);
    else                    isec.uv = Vector2(p.x, 1.0f-p.y);
}

Vector3 AABox::getMin() const
{
    return data[0];
}

void AABox::setMin(const Vector3 &value)
{
    data[0] = value;
}

AABB AABox::getAABB() const
{
    return AABB(data[0], data[1]);
}

GBox::GBox(): Instance(unitBox)
{
    material = material::DiffuseWhite;
}

void GBox::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : material::DiffuseWhite;
}

const Material *GBox::getMaterial(size_t) const
{
    return material.get();
}
