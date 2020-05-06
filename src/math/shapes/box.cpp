#include "box.h"
#include "shapefactory.h"

AABox::AABox(const Vector3 &min, const Vector3 &max): min(min), max(max)
{

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
    Vector3 invdir = 1.0f / ray.direction ;

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
Vector3 AABox::normal(const Vector3 &, size_t idx) const
{
    if (idx == 1)      return Vector3(-1,0,0);
    else if (idx == 2) return Vector3(1,0,0);
    else if (idx == 3) return Vector3(0,-1,0);
    else if (idx == 4) return Vector3(0,1,0);
    else if (idx == 5) return Vector3(0,0,1);
    else               return Vector3(0,0,-1);
}

inline
Vector2 AABox::uv(const Vector3 &phit, size_t idx) const
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


void AABox::fetch(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = normal(isec.phit, isec.idx);
    isec.material = mat.get();

    if(mat->texture)
    {
        isec.uv = uv(isec.phit, isec.idx);
        isec.color = mat->texture->get(isec.uv) * mat->Kd;
    }
    else
        isec.color = mat->Kd;
}
