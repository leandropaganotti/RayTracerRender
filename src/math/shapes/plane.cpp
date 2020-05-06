#include "plane.h"

Plane::Plane(const Vector3 &O, const Vector3 &N):
    O(O), N(N)
{

}

Vector3 Plane::getN() const
{
    return N;
}

void Plane::setN(const Vector3 &value)
{
    N = value;
}

Vector3 Plane::getO() const
{
    return O;
}

void Plane::setO(const Vector3 &value)
{
    O = value;
}

bool Plane::intersection(const Ray &ray, float tmax, IntersectionData& isec) const
{
    const float t = ((O-ray.origin) ^ N) / (ray.direction ^ N);

    if( t < 0.0f || t > tmax) return false;

    isec.tnear = t;
    return true;
}

bool Plane::intersection(const Ray &ray, float tmax) const
{
    const float t = ((O-ray.origin) ^ N) / (ray.direction ^ N);
    return ( t > 0.0f && t < tmax) ? true : false;
}

Vector3 Plane::normal(const Vector3 &, size_t) const
{
    return N;
}

Vector2 Plane::uv(const Vector3 &phit, size_t) const
{

    Vector3 v = phit - O;

    if(N.x == 1.0f || N.x == -1.0f)
    {
        return {v.y, v.z};
    }
    else if(N.y == 1.0f || N.y == -1.0f)
    {
        return {v.x, v.z};
    }
    else
    {
        return {v.x, v.y};
    }
}

void Plane::fetch(const Ray &ray, IntersectionData &isec) const
{
    isec.phit = ray.origin + isec.tnear * ray.direction;
    isec.normal = N;
    isec.material = mat.get();

    if(mat->texture)
    {
        Vector3 v = isec.phit - O;
        if(N.x == 1.0f || N.x == -1.0f)
        {
            isec.uv = {v.y, v.z};
        }
        else if(N.y == 1.0f || N.y == -1.0f)
        {
            isec.uv = {v.x, v.z};
        }
        else
        {
            isec.uv = {v.x, v.y};
        }
        isec.color = mat->texture->get(isec.uv) * mat->Kd;
    } else
        isec.color = mat->Kd;
}
