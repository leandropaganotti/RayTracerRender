#include "plane.h"

Plane::Plane(const Vector3f &P, const Vector3f &n, const Vector3f& color):
    Object(color), P(P), N(n)
{
	tex.reset(new ChessBoard(4, 4, 0, 1));
}

bool Plane::intersection(const Ray &ray, IntersectionData& isec) const
{
    if( intersection(P, N, ray, isec.tnear) )
    {
        isec.object = this;
        return true;
    }
    return false;
}

bool Plane::intersection(const Ray &ray, float& tnear) const
{
    return intersection(P, N, ray, tnear);
}

const Vector3f Plane::normal(const Vector3f &, size_t) const
{
    return N;
}

inline
bool Plane::intersection(const Vector3f &P, const Vector3f &n, const Ray &ray, float &tnear)
{
    float t = ((P-ray.origin) ^ n) / (ray.direction ^ n);

    if ( t > 0.0f )
    {
        tnear = t;
        return true;
    }
    return false;
}

const Vector3f Plane::texture(const Vector3f& phit) const
{
	if(tex)
	{
		return tex->get(phit.x, phit.z);
	}
	return Vector3f(1.0f);
}
