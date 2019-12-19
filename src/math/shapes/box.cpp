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

bool AABox::intersection(const Ray &ray, IntersectionData &isec) const
{
    Vector3 invdir = 1.0f / ray.direction ;

    float t1 = (min.x - ray.origin.x)*invdir.x;
    float t2 = (max.x - ray.origin.x)*invdir.x;
    float t3 = (min.y - ray.origin.y)*invdir.y;
    float t4 = (max.y - ray.origin.y)*invdir.y;
    float t5 = (min.z - ray.origin.z)*invdir.z;
    float t6 = (max.z - ray.origin.z)*invdir.z;

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
    return true;
}

bool AABox::intersection(const Ray &ray, float &tnear) const
{
    Vector3 invdir = 1.0f / ray.direction;

    float t1 = (min.x - ray.origin.x)*invdir.x;
    float t2 = (max.x - ray.origin.x)*invdir.x;
    float t3 = (min.y - ray.origin.y)*invdir.y;
    float t4 = (max.y - ray.origin.y)*invdir.y;
    float t5 = (min.z - ray.origin.z)*invdir.z;
    float t6 = (max.z - ray.origin.z)*invdir.z;

    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    if (tmax < 0 || tmin > tmax) { return false; }

    if(tmin < 0) { tnear = tmax; } else { tnear = tmin; }
    return true;
}

Vector3 AABox::normal(const Vector3 &, size_t idx) const
{
    if (idx == 1)      return Vector3(-1,0,0);
    else if (idx == 2) return Vector3(1,0,0);
    else if (idx == 3) return Vector3(0,-1,0);
    else if (idx == 4) return Vector3(0,1,0);
    else if (idx == 5) return Vector3(0,0,1);
    else               return Vector3(0,0,-1);
}

std::pair<float, float> AABox::uv(const Vector3 &phit, size_t idx) const
{
    float u=0, v=0;
    if (idx == 1 || idx == 2)
    {
        u = phit.z; v = phit.y;
    } else if (idx == 3 || idx == 4)
    {
        u = phit.x; v = phit.z;
    } else
    {
        u = phit.x; v = phit.y;
    }
    return std::make_pair(u, v);
}

Vector3 AABox::getMin() const
{
    return min;
}

void AABox::setMin(const Vector3 &value)
{
    min = value;
}

Box::Box()
{
    shape = Shapes::UnitBox;
}
