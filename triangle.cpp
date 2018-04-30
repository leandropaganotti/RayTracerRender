#include "triangle.h"
#include <vertex.h>

Triangle::Triangle(size_t v0, size_t v1, size_t v2, const Vector3f &n, const Vector3f &color):
    Object(color), v{v0, v1, v2}, normal(n)
{
    size_t vbs = vertexBuffer.size();
    if(v0 >= vbs || v1 >= vbs || v2 >= vbs)
        throw std::out_of_range("Vertex index is out of Range");

    area = ((vertexBuffer[v1]-vertexBuffer[v0]) % (vertexBuffer[v2]-vertexBuffer[v0])).length() / 2.0f;
}

inline
bool Triangle::intersection(const Ray &ray, IntersectionData &inter) const
{
    if (Plane::intersection(vertexBuffer[v[0]], normal, ray, inter.dist))
    {
        inter.phit = ray.origin + inter.dist * ray.direction;

        if (isInside2(inter.phit))
        {
            inter.normal = normal;
            inter.object = this;
            return true;
        }
    }
    return false;
}

inline
bool Triangle::intersection(const Ray &ray, float &dist) const
{
    if (Plane::intersection(vertexBuffer[v[0]], normal, ray, dist))
    {
        Vector3f phit = ray.origin + dist * ray.direction;
        return isInside2(phit);
    }
    return false;
}


bool Triangle::intersection(const Ray &ray, Vector3f &phit, float & dist) const
{
    if (Plane::intersection(vertexBuffer[v[0]], normal, ray, dist))
    {
        phit = ray.origin + dist * ray.direction;
        return isInside2(phit);
    }
    return false;
}

std::ostream &operator <<(std::ostream &os, const Triangle &t)
{
    return os << t.v[0] << "->" << vertexBuffer[t.v[0]] << t.v[1] << "->" << vertexBuffer[t.v[1]] << t.v[2] << "->" << vertexBuffer[t.v[2]] << " " << t.normal << " " << t.area;
}

inline
bool Triangle::isInside2(const Vector3f &P) const // area
{
    Vector3f PA = vertexBuffer[v[0]]-P;
    Vector3f PB = vertexBuffer[v[1]]-P;
    Vector3f PC = vertexBuffer[v[2]]-P;
    float a1 = ((PA) % (PB)).length()/2.0f ;// PAB
    float a2 = ((PA) % (PC)).length()/2.0f ;// PAC
    float a3 = ((PB) % (PC)).length()/2.0f ;// PBC
    return fabs(area-a1-a2-a3) < 0.0001f;
}

inline
bool Triangle::isInside(const Vector3f &P) const
{
    Vector3f e0 = vertexBuffer[v[1]] - vertexBuffer[v[0]];
    Vector3f e1 = vertexBuffer[v[2]] - vertexBuffer[v[1]];
    Vector3f e2 = vertexBuffer[v[0]] - vertexBuffer[v[2]];
    Vector3f C0 = P - vertexBuffer[v[0]];
    Vector3f C1 = P - vertexBuffer[v[1]];
    Vector3f C2 = P - vertexBuffer[v[2]];
    return ((normal ^ (e0 % C0)) > 0 && (normal ^ (e1 % C1)) > 0 && (normal ^ (e2 % C2)) > 0); // P is inside the triangle
}
