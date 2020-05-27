#include "box.h"
#include "material.h"
#include "float.h"

static std::shared_ptr<AABox> unitBox = std::shared_ptr<AABox>(new AABox);

AABox::AABox(const Vector3 &min, const Vector3 &max): min(min), max(max)
{
    data[0] = {-0.5, -0.5, -0.5};
    data[1] = {0.5, 0.5, 0.5};
}

Vector3 AABox::getMax() const
{
    return max;
}

void AABox::setMax(const Vector3 &value)
{
    max = value;
}

bool AABox::intersection(const Ray &ray, float tmax, IntersectionData &isec) const
{
  /* Vector3 invdir = 1.0f / ray.direction;

   float t1 = (min.x - ray.origin.x)*invdir.x;
   float t2 = (max.x - ray.origin.x)*invdir.x;
   float t3 = (min.y - ray.origin.y)*invdir.y;
   float t4 = (max.y - ray.origin.y)*invdir.y;
   float t5 = (min.z - ray.origin.z)*invdir.z;
   float t6 = (max.z - ray.origin.z)*invdir.z;

   float tminx, tminy, tminz, tmin_, tmax_;
   size_t idxx, idxy, idxz, idx;

   if(t1 < t2) { idxx = 1; tminx = t1; } else { idxx = 2; tminx = t2; }

   if(t3 < t4) { idxy = 3; tminy = t3; } else { idxy = 4; tminy = t4; }

   if(t5 < t6) { idxz = 5; tminz = t5; } else { idxz = 6; tminz = t6; }

   if ( tminx > tminy ) { tmin_ = tminx; idx = idxx; } else { tmin_ = tminy; idx = idxy; }

   if ( tminz > tmin_ ) { tmin_ = tminz; idx = idxz; }

   tmax_ = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

   // if tmax < 0, it is behind the ray
   if (tmax_ < 0) { return false; }

   // if tmin > tmax, ray doesn't intersect
   if (tmin_ > tmax_) { return false; }

   float tval;
   // if tmin < 0 then the ray origin is inside of the box and tmin is behind the start of the ray so tmax is the first intersection
   if(tmin_ < 0) { tval = tmax_; } else { tval = tmin_; }

   if (tval > tmax) return false;

   isec.tnear = tval;
   isec.idx = idx;
   return true;
*/

    Ray r = ray;
    r.invdir = 1.0f / ray.direction;
    r.posneg[0] = r.direction[0] > 0 ? 0 : 1;
    r.posneg[1] = r.direction[1] > 0 ? 0 : 1;
    r.posneg[2] = r.direction[2] > 0 ? 0 : 1;

    float imin = -1000000;
    float imax = tmax;
    int idxmin, idxmax;

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

bool AABox::intersection(const Ray &ray, float tmax) const
{
    Vector3 invdir = 1.0f / ray.direction;

    float t1 = (min.x - ray.origin.x)*invdir.x;
    float t2 = (max.x - ray.origin.x)*invdir.x;
    float t3 = (min.y - ray.origin.y)*invdir.y;
    float t4 = (max.y - ray.origin.y)*invdir.y;
    float t5 = (min.z - ray.origin.z)*invdir.z;
    float t6 = (max.z - ray.origin.z)*invdir.z;

    float tmin_ = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax_ = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax_ < 0 || tmin_ > tmax_) { return false; }

    float tval;
    if(tmin_ < 0) { tval = tmax_; } else { tval = tmin_; }

    if (tval > tmax) return false;

    return true;
}

inline
Vector3 AABox::getNormal(const Vector3 &, size_t idx) const
{
    if (idx == 0)      return Vector3(-1,0,0);
    else if (idx == 1) return Vector3(1,0,0);
    else if (idx == 2) return Vector3(0,-1,0);
    else if (idx == 3) return Vector3(0,1,0);
    else if (idx == 4) return Vector3(0,0,-1);
    else               return Vector3(0,0,1);
}

inline
Vector2 AABox::getUV(const Vector3 &phit, size_t idx) const
{
    if (idx == 1 || idx == 2)
    {
        return {phit.z, phit.y};
    } else if (idx == 3 || idx == 4)
    {
        return {phit.x, phit.z};
    } else
    {
        return {phit.x, phit.y};
    }
}

Vector3 AABox::getMin() const
{
    return min;
}

void AABox::setMin(const Vector3 &value)
{
    min = value;
}

AABB AABox::getAABB() const
{
    return AABB(min, max);
}

GBox::GBox(): Instance(unitBox)
{
    material = material::DiffuseWhite;
}

void GBox::getIsecData(const Ray &ray, IntersectionData &isec) const
{
    Instance::getIsecData(ray, isec);
    isec.material = material.get();
    isec.color = material->Kd;
    if(material->texture)
    {
        isec.uv = static_cast<AABox*>(shape.get())->getUV(isec.phit_local, isec.idx);
        isec.color = isec.color * material->texture->get(isec.uv);
    }
}

void GBox::setMaterial(const std::shared_ptr<Material> &value)
{
    material = value ? value : material::DiffuseWhite;
}

const Material *GBox::getMaterial(size_t) const
{
    return material.get();
}
